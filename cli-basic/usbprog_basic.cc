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
#include <iostream>
#include <sstream>

#include <usbprog-core/devices.h>
#include <usbprog-core/util.h>
#include <usbprog-core/types.h>
#include "usbprog_basic.h"

namespace usbprog {
namespace cli_basic {

/* UsbprogBasic {{{ */

UsbprogBasic::UsbprogBasic(int argc, char *argv[])
    : m_argc(argc)
    , m_argv(argv)
{}

UsbprogBasic::~UsbprogBasic()
{}

Action UsbprogBasic::parseCommandLine(int &deviceNumber, std::string &fw) const
{
    // std::string is easier to use
    core::StringVector args;
    for (int i = 1; i < m_argc; i++)
        args.push_back(std::string(m_argv[i]));

    if (args.size() == 0)
        return ACTION_ERROR;

    // explicitely request help
    if (args[0] == "-h" || args[0] == "--help" || args[0] == "/?")
        return ACTION_PRINT_HELP;

    // the list command
    if (args[0] == "list")
        return ACTION_LIST_DEVICES;

    // the upload firmware
    if (args[0] == "upload") {
        if (args.size() == 2 || args.size() == 3) {
            deviceNumber = 0;
            fw = args[1];
            if (args.size() == 3) {
                std::stringstream ss;
                ss << args[2];
                ss >> deviceNumber;
            }
            return ACTION_UPLOAD_FIRMWARE;
        } else {
            std::cerr << "The command 'upload' requires (exactly) one or two arguments.\n"
                      << std::endl;
            return ACTION_ERROR;
        }
    }

    return ACTION_ERROR;
}

int UsbprogBasic::exec()
{
    int deviceNumber;
    std::string firmwareFile;

    Action action = parseCommandLine(deviceNumber, firmwareFile);
    switch (action) {
        case ACTION_PRINT_HELP:
            printHelp();
            return RC_SUCCESS;

        case ACTION_ERROR:
            printHelp();
            return RC_INVALID_COMMANDLINE;

        case ACTION_LIST_DEVICES:
            return listDevices();

        case ACTION_UPLOAD_FIRMWARE:
            return uploadFirmware(deviceNumber, firmwareFile);

        default:
            std::cerr << "Invalid action." << std::endl;
            return RC_OTHER_ERROR;
    }
}

void UsbprogBasic::printHelp() const
{
    std::cout << "Usage: usbprog-basic [-h] <command> [args...]\n"
              << "\n"
              << "Where command can be one of following:\n"
              << "  list      : Lists all available USBprog devices.\n"
              << "  upload    : Uploads a new firmware to the device.\n"
              << "              If only one argument is specified, that argument represents the\n"
              << "              firmware file. If two arguments are specified, the first one must\n"
              << "              be the firmware file and the second must be the device number\n"
              << "              (printed by 'list').\n"
              << "Examples:\n"
              << " (1) usbprog-basic list\n"
              << " (2) uspborg-basic upload blinkdemo.bin\n"
              << " (3) usbprog-basic upload blinkdemo.bin 1" << std::endl;
}

ErrorCode UsbprogBasic::listDevices() const
{
    try {
        core::DeviceManager deviceManager;
        deviceManager.discoverUpdateDevices();
        deviceManager.printDevices(std::cout, false);
    } catch (const core::IOError &err) {
        std::cerr << "I/O Error: " << err.what() << std::endl;
        return RC_IOERROR;
    }

    return RC_SUCCESS;
}

ErrorCode UsbprogBasic::uploadFirmware(int deviceNumber, const std::string &firmwareFile) const
{
    core::DeviceManager deviceManager;
    deviceManager.discoverUpdateDevices();

    //
    // get the update device
    //

    if (deviceNumber < 0 || deviceNumber > deviceManager.getNumberUpdateDevices()) {
        std::cerr << "Device number is invalid. Use 'list' to list all"
                  << "available devices." << std::endl;
        return RC_DEV_NOT_FOUND;
    }

    core::Device *updateDevice = deviceManager.getDevice(deviceNumber);
    if (!updateDevice) {
        std::cerr << "Unable to get update device number '" << deviceNumber << "'." << std::endl;
        return RC_DEV_NOT_FOUND;
    }

    //
    // read the firmware file
    //

    core::ByteVector firmwareData;
    try {
        firmwareData = core::Fileutil::readBytesFromFile(firmwareFile);
    } catch (const core::IOError &err) {
        std::cerr << "Unable to read '" << firmwareFile << "'." << std::endl;
        return RC_FILE_NOT_EXIST;
    }

    core::UsbprogUpdater updater(updateDevice);
    try {
        std::cout << "Opening device..." << std::endl;
        updater.updateOpen();

        std::cout << "Writing firmware..." << std::endl;
        updater.writeFirmware(firmwareData);

        std::cout << "Starting device..." << std::endl;
        updater.startDevice();
        updater.updateClose();
    } catch (const core::IOError &err) {
        std::cerr << "I/O Error: " << err.what() << std::endl;
        return RC_IOERROR;
    }

    return RC_SUCCESS;
}

/* }}} */

} // end namespace cli
} // end namespace cli_basic

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
