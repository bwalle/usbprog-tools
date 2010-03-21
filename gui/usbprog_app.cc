/*
 * (c) 2010, Bernhard Walle <bernhard@bwalle.de>
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
#include <cstdlib>

#include <boost/program_options.hpp>

#include <usbprog-core/debug.h>
#include <usbprog-core/util.h>

#include "usbprog_app.h"
#include "guiconfiguration.h"
#include "config.h"

namespace po = boost::program_options;

/* UsbprogApplication {{{ */

// -----------------------------------------------------------------------------
const QString UsbprogApplication::NAME = "USBprog";
const QString UsbprogApplication::USBPROG_WEBSITE = "http://www.embedded-projects.net/index.php?page_id=135";

// -----------------------------------------------------------------------------
UsbprogApplication::UsbprogApplication(int &argc, char **argv)
        throw (ApplicationError)
    : QApplication(argc, argv)
    , m_mainWindow(NULL)
{
    initConfig();
}

// -----------------------------------------------------------------------------
void UsbprogApplication::initConfig()
    throw (ApplicationError)
{
    GuiConfiguration *conf = GuiConfiguration::config();

    std::string configDir = Fileutil::configDir("usbprog");
    if (configDir.size() == 0)
        throw ApplicationError("Could not determine configuration directory.");

    conf->setDataDir(configDir);
    conf->setIndexUrl(DEFAULT_INDEX_URL);
}

// -----------------------------------------------------------------------------
bool UsbprogApplication::parseCommandLine(int argc, char **argv, int &exitCode)
{
    GuiConfiguration *conf = GuiConfiguration::config();

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h",      "Prints a help message")
        ("version,v",   "Prints version information")
        ("datadir,d",   po::value<std::string>(),
                        ("Uses the specified data directory instead of " + conf->getDataDir()).c_str())
        ("offline,o",   "Use only the local cache and don't connect to the internet")
        ("debug,D",     "Enables debug output");

    po::options_description hidden("Hidden options");
    hidden.add_options()
        ("commands", po::value< std::vector<std::string> >(), "Commands");

    po::options_description commandline_options;
    commandline_options.add(desc).add(hidden);

    po::positional_options_description p;
    p.add("commands", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).
                  options(commandline_options).positional(p).run(), vm);
    } catch (const po::error &err) {
        std::cerr << "Parsing command line failed: " << err.what() << std::endl;
        exitCode = EXIT_FAILURE;
        return false;
    }
    po::notify(vm);

    if (vm.count("debug")) {
        conf->setDebug(true);
        Debug::debug()->setLevel(Debug::DL_TRACE);
    }

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        exitCode = EXIT_SUCCESS;
        return false;
    }

    if (vm.count("version")) {
        std::cerr << "usbprog " << USBPROG_VERSION_STRING << std::endl;
        exitCode = EXIT_SUCCESS;
        return false;
    }

    if (vm.count("datadir"))
        conf->setDataDir(vm["datadir"].as<std::string>());
    if (vm.count("offline"))
        conf->setOffline(true);

    if (conf->getDebug())
        conf->dumpConfig(std::cerr);

    if (conf->isOffline())
        std::cout << "WARNING: You're using usbprog in offline mode!" << std::endl;

    return true;
}

// -----------------------------------------------------------------------------
void UsbprogApplication::createAndSetMainWidget()
{
    m_mainWindow = new UsbprogMainWindow;
    m_mainWindow->show();
}


/* }}} */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
