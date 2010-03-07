/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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

#include <boost/program_options.hpp>

#include <usbprog-core/devices.h>
#include <usbprog-core/util.h>
#include <usbprog-core/debug.h>
#include <usbprog/firmwarepool.h>
#include <usbprog/usbprog.h>

#include "usbprog.h"
#include "configuration.h"
#include "shell.h"
#include "commands.h"
#include "config.h"

namespace po = boost::program_options;

using std::string;
using std::vector;
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::exit;
using std::runtime_error;

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
        cout << '#';
        m_lastProgress++;
    }

    cout << flush;

    return true;
}

/* -------------------------------------------------------------------------- */
void HashNotifier::finished()
{
    if (m_lastProgress != 0) {
        cout << endl;
        m_lastProgress = 0;
    }
}

/* }}} */
/* Usbprog {{{ */

/* -------------------------------------------------------------------------- */
Usbprog::Usbprog(int argc, char **argv)
    : m_coreApp(argc, argv)
    , m_firmwarepool(NULL)
    , m_progressNotifier(NULL)
    , m_argc(argc)
    , m_argv(argv)
{}

/* -------------------------------------------------------------------------- */
Usbprog::~Usbprog()
{
    delete m_firmwarepool;
    delete m_progressNotifier;
}

/* -------------------------------------------------------------------------- */
void Usbprog::initConfig()
    throw (ApplicationError)
{
    Configuration *conf = Configuration::config();

    string configDir = Fileutil::configDir("usbprog");
    if (configDir.size() == 0)
        throw ApplicationError("Could not determine configuration "
                "directory.");

    conf->setDataDir(configDir);
    conf->setHistoryFile(pathconcat(configDir, "history"));
    conf->setIndexUrl(DEFAULT_INDEX_URL);
}

/* -------------------------------------------------------------------------- */
void Usbprog::parseCommandLine()
{
    Configuration *conf = Configuration::config();

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Prints a help message")
        ("version,v", "Prints version information")
        ("datadir,d", po::value<string>(), string("Uses the specified data "
            "directory instead of " + conf->getDataDir()).c_str())
        ("offline,o", "Use only the local cache "
            "and don't connect to the internet")
        ("debug,D", "Enables debug output");

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("commands", po::value< vector<string> >(), "Commands");

    po::options_description commandline_options;
    commandline_options.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("commands", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(m_argc, m_argv).
                  options(commandline_options).positional(p).run(), vm);
    } catch (const po::error &err) {
        throw ApplicationError("Parsing command line failed: " + string(err.what()));
    }
    po::notify(vm);    

    if (vm.count("debug")) {
        conf->setDebug(true);
        Debug::debug()->setLevel(Debug::DL_TRACE);
    }

    if (vm.count("help")) {
        cout << desc << endl;
        exit(EXIT_SUCCESS);
    }

    if (vm.count("version")) {
        cerr << "usbprog " << USBPROG_VERSION_STRING << endl;
        exit(EXIT_SUCCESS);
    }

    if (vm.count("datadir"))
        conf->setDataDir(vm["datadir"].as<string>());
    if (vm.count("offline"))
        conf->setOffline(true);

    if (conf->getDebug())
        conf->dumpConfig(cerr);

    // batch mode?
    conf->setBatchMode(vm.count("commands") != 0);
    if (conf->getBatchMode())
        m_args = vm["commands"].as< vector<string> >();

    if (conf->isOffline() && !conf->getBatchMode())
        cout << "WARNING: You're using usbprog in offline mode!" << endl;

    if (!conf->getBatchMode())
        m_progressNotifier = new HashNotifier(DEFAULT_TERMINAL_WIDTH);
}

/* -------------------------------------------------------------------------- */
void Usbprog::initFirmwarePool()
    throw (ApplicationError)
{
    Configuration *conf = Configuration::config();

    try {
        m_firmwarepool = new Firmwarepool(conf->getDataDir());
        m_firmwarepool->setIndexUpdatetime(AUTO_NOT_UPDATE_TIME);
        if (!conf->isOffline())
            m_firmwarepool->downloadIndex(conf->getIndexUrl());
        if (!conf->getDebug())
            m_firmwarepool->setProgress(m_progressNotifier);
        m_firmwarepool->readIndex();
    } catch (const runtime_error &re) {
        throw ApplicationError(re.what());
    }
}

/* -------------------------------------------------------------------------- */
void Usbprog::initDeviceManager()
    throw (ApplicationError)
{
    bool debug = Configuration::config()->getDebug();
    m_devicemanager = new DeviceManager(debug ? 1 : 0);
}

/* -------------------------------------------------------------------------- */
void Usbprog::exec()
    throw (ApplicationError)
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
    if (Configuration::config()->getBatchMode())
        sh.run(m_args);
    else
        sh.run();
}

/* }}} */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1: