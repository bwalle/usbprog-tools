/*
 * (c) 2007-2010, Robert Schilling
 *                Bernhard Walle <bernhard@bwalle.de>
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
#include <cstdio>
#include <cstdlib>

#include <boost/program_options.hpp>

#include "usbprogApp.h"
#include "usbprogFrm.h"
#ifdef __APPLE__
#include <ApplicationServices/ApplicationServices.h>
#endif

namespace po = boost::program_options;

using std::cout;
using std::cerr;
using std::endl;
using std::exit;

IMPLEMENT_APP(usbprogFrmApp)

/* -------------------------------------------------------------------------- */
void parse_command_line(int argc, wxChar **argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Prints a help message")
        ("version,v", "Prints version information")
        ("debug,D", "Enables debug output");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
    } catch (const po::error &err) {
        std:: cerr << "Parsing command line failed: " << err.what() << std::endl;
    }
    po::notify(vm);

    if (vm.count("debug"))
        Debug::debug()->setLevel(Debug::DL_TRACE);

    if (vm.count("help")) {
        cout << desc << endl;
        exit(EXIT_SUCCESS);
    }

    if (vm.count("version")) {
        cerr << "usbprog-gui " << USBPROG_VERSION_STRING << endl;
        exit(EXIT_SUCCESS);
    }
}

/* -------------------------------------------------------------------------- */
bool usbprogFrmApp::OnInit()
{
    parse_command_line(argc, argv);

#ifdef __APPLE__
    // this is necessary because of
    // http://wiki.wxwidgets.org/WxMac_Issues#My_app_can.27t_be_brought_to_the_front.21
    ProcessSerialNumber PSN;
    GetCurrentProcess(&PSN);
    TransformProcessType(&PSN, kProcessTransformToForegroundApplication);
#endif

    usbprogFrm* frame = new usbprogFrm(NULL);
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}

/* -------------------------------------------------------------------------- */
int usbprogFrmApp::OnExit()
{
	return 0;
}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
