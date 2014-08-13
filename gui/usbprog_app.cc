/*
 * (c) 2011, Bernhard Walle <bernhard@bwalle.de>
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

#include <usbprog-core/debug.h>
#include <usbprog-core/util.h>

#include <libbw/optionparser.h>

#include "usbprog_app.h"
#include "guiconfiguration.h"
#include "config.h"

namespace usbprog {
namespace gui {

/* UsbprogApplication {{{ */

// -----------------------------------------------------------------------------
const QString UsbprogApplication::NAME = "USBprog";
const QString UsbprogApplication::USBPROG_WEBSITE = "http://www.embedded-projects.net/index.php?page_id=135";

// -----------------------------------------------------------------------------
UsbprogApplication::UsbprogApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_mainWindow(NULL)
{
    initConfig();
}

// -----------------------------------------------------------------------------
void UsbprogApplication::initConfig()
{
    GuiConfiguration &conf = GuiConfiguration::config();

    std::string configDir = core::Fileutil::configDir("usbprog");
    if (configDir.size() == 0)
        throw core::ApplicationError("Could not determine configuration directory.");

    conf.setDataDir(configDir);
    conf.setIndexUrl(DEFAULT_INDEX_URL);
}

// -----------------------------------------------------------------------------
bool UsbprogApplication::parseCommandLine(int argc, char **argv, int &exitCode)
{
    GuiConfiguration &conf = GuiConfiguration::config();

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

    if (!op.parse(argc, argv))
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

    if (conf.isOffline())
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

} // end namespace gui
} // end namespace usbprog


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
