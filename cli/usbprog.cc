/*
 * (c) 2007-2011, Bernhard Walle <bernhard@bwalle.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <usbprog-core/devices.h>
#include <usbprog-core/util.h>
#include <usbprog-core/debug.h>
#include <usbprog/firmwarepool.h>
#include <usbprog/usbprog.h>

#include <libbw/optionparser.h>

#include "usbprog.h"
#include "cliconfiguration.h"
#include "shell.h"
#include "commands.h"
#include "config.h"

namespace usbprog {
namespace cli {

/* HashNotifier {{{ */

/* -------------------------------------------------------------------------- */
HashNotifier::HashNotifier(int width)
    : m_width(width), m_lastProgress(0)
{}

/* -------------------------------------------------------------------------- */
HashNotifier::~HashNotifier()
{
    if (m_lastProgress != 0)
        finished();
}

/* -------------------------------------------------------------------------- */
int HashNotifier::progressed(double total, double now)
{
    /* zero division */
    if (total < 0.01)
        return true;

    double percent = now / total;
    int bars = int(percent * m_width);
    while (bars > m_lastProgress) {
        std::cout << '#';
        m_lastProgress++;
    }

    std::cout << std::flush;

    return true;
}

/* -------------------------------------------------------------------------- */
void HashNotifier::finished()
{
    if (m_lastProgress != 0) {
        std::cout << std::endl;
        m_lastProgress = 0;
    }
}

/* }}} */
/* Usbprog {{{ */

/* -------------------------------------------------------------------------- */
Usbprog::Usbprog(int argc, char *argv[])
    : m_coreApp(argc, argv)
    , m_firmwarepool(NULL)
    , m_devicemanager(NULL)
    , m_progressNotifier(NULL)
    , m_argc(argc)
    , m_argv(argv)
{}

/* -------------------------------------------------------------------------- */
Usbprog::~Usbprog()
{
    delete m_firmwarepool;
    delete m_progressNotifier;
    delete m_devicemanager;
}

/* -------------------------------------------------------------------------- */
void Usbprog::initConfig()
    throw (core::ApplicationError)
{
    CliConfiguration &conf = CliConfiguration::config();

    std::string configDir = core::Fileutil::configDir("usbprog");
    if (configDir.size() == 0)
        throw core::ApplicationError("Could not determine configuration "
                "directory.");

    conf.setDataDir(configDir);
    conf.setHistoryFile(core::pathconcat(configDir, "history"));
    conf.setIndexUrl(DEFAULT_INDEX_URL);
}

/* -------------------------------------------------------------------------- */
void Usbprog::parseCommandLine()
{
    CliConfiguration &conf = CliConfiguration::config();

    bw::OptionParser op;
    op.addOption("debug",   'D', bw::OT_FLAG,
                 "Enables debugging output");
    op.addOption("help",    'h', bw::OT_FLAG,
                 "Prints a help message");
    op.addOption("datadir", 'd', bw::OT_STRING,
                 "Uses the specified data " "directory instead of " + conf.getDataDir());
    op.addOption("offline", 'o', bw::OT_FLAG,
                 "Use only the local cache and don't connect to the internet");
    op.addOption("debug",   'D', bw::OT_FLAG,
                 "Enables debug output");

    if (!op.parse(m_argc, m_argv))
        throw core::ApplicationError("Parsing command line failed.");

    if (op.getValue("debug").getFlag()) {
        conf.setDebug(true);
        core::Debug::debug()->setLevel(core::Debug::DL_TRACE);
    }

    if (op.getValue("version").getFlag()) {
        std::cerr << "usbprog " << USBPROG_VERSION_STRING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if (op.getValue("datadir").getType() != bw::OT_INVALID)
        conf.setDataDir(op.getValue("datadir").getString());
    if (op.getValue("offline").getFlag())
        conf.setOffline(true);

    if (conf.getDebug())
        conf.dumpConfig(std::cerr);

    // batch mode?
    std::vector<std::string> args = op.getArgs();
    conf.setBatchMode(args.size() > 0);
    if (conf.getBatchMode())
        m_args = args;

    if (conf.isOffline() && !conf.getBatchMode())
        std::cout << "WARNING: You're using usbprog in offline mode!" << std::endl;

    if (!conf.getBatchMode())
        m_progressNotifier = new HashNotifier(DEFAULT_TERMINAL_WIDTH);
}

/* -------------------------------------------------------------------------- */
void Usbprog::initFirmwarePool()
    throw (core::ApplicationError)
{
    CliConfiguration &conf = CliConfiguration::config();

    try {
        m_firmwarepool = new Firmwarepool(conf.getDataDir());
        m_firmwarepool->setIndexUpdatetime(AUTO_NOT_UPDATE_TIME);
        if (!conf.isOffline())
            m_firmwarepool->downloadIndex(conf.getIndexUrl());
        if (!conf.getDebug())
            m_firmwarepool->setProgress(m_progressNotifier);
        m_firmwarepool->readIndex();
    } catch (const std::runtime_error &re) {
        throw core::ApplicationError(re.what());
    }
}

/* -------------------------------------------------------------------------- */
void Usbprog::initDeviceManager()
    throw (core::ApplicationError)
{
    bool debug = CliConfiguration::config().getDebug();
    m_devicemanager = new core::DeviceManager(debug);
}

/* -------------------------------------------------------------------------- */
void Usbprog::exec()
    throw (core::ApplicationError)
{
    Shell sh("(usbprog) ");

    sh.addCommand(new CopyingCommand);
    sh.addCommand(new ListCommand(m_firmwarepool));
    sh.addCommand(new InfoCommand(m_firmwarepool));
    sh.addCommand(new PinCommand(m_firmwarepool));
    sh.addCommand(new DownloadCommand(m_firmwarepool));
    sh.addCommand(new CacheCommand(m_firmwarepool));
    sh.addCommand(new DevicesCommand(m_devicemanager, m_firmwarepool));
    sh.addCommand(new DeviceCommand(m_devicemanager, m_firmwarepool));
    sh.addCommand(new UploadCommand(m_devicemanager, m_firmwarepool));
    sh.addCommand(new StartCommand(m_devicemanager));
    sh.addCommand(new ResetCommand(m_devicemanager));
    if (CliConfiguration::config().getBatchMode())
        sh.run(m_args);
    else
        sh.run();
}

/* }}} */

} // end namespace cli
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
