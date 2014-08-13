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
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <vector>
#include <fstream>

#include <usbprog-core/stringutil.h>
#include <usbprog-core/util.h>
#include <usbprog/firmwarepool.h>

#include "commands.h"
#include "cliconfiguration.h"
#include "usbprog.h"
#include "config.h"

namespace usbprog {
namespace cli {

/* functions {{{ */

/* -------------------------------------------------------------------------- */
core::StringVector complete_firmware(const std::string &start, Firmwarepool *pool)
{
    core::StringVector result;
    StringList firmwarelist = pool->getFirmwareNameList();

    for (StringList::const_iterator it = firmwarelist.begin();
            it != firmwarelist.end(); ++it) {
        std::string fwname = *it;
        if (core::str_starts_with(fwname, start))
            result.push_back(fwname);
    }

    return result;
}


/* }}} */
/* ListCommand {{{ */

/* -------------------------------------------------------------------------- */
ListCommand::ListCommand(Firmwarepool *firmwarepool)
    : AbstractCommand("list"), m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool ListCommand::execute(CommandArgVector   args,
                          core::StringVector options,
                          std::ostream       &os)
{
    StringList firmwarelist = m_firmwarepool->getFirmwareNameList();

    size_t maxSize = 0;
    for (StringList::const_iterator it = firmwarelist.begin();
            it != firmwarelist.end(); ++it)
        maxSize = std::max(maxSize, it->size());

    maxSize += 2;

    for (StringList::const_iterator it = firmwarelist.begin();
            it != firmwarelist.end(); ++it) {
        Firmware *fw = m_firmwarepool->getFirmware(*it);
        os << std::left << std::setw(maxSize) << fw->getName();
        if (m_firmwarepool->isFirmwareOnDisk(fw->getName()))
            os << "[*] ";
        else
            os << "[ ] ";
        os << fw->getLabel() << std::endl;
    }

    if (!CliConfiguration::config().getBatchMode())
        os << std::endl << "*: Firmware file downloaded" << std::endl;

    return true;
}

/* -------------------------------------------------------------------------- */
core::StringVector ListCommand::aliases() const
{
    core::StringVector ret;
    ret.push_back("firmwares");
    return ret;
}

/* -------------------------------------------------------------------------- */
std::string ListCommand::help() const
{
    return "Lists all available firmwares.";
}

/* -------------------------------------------------------------------------- */
void ListCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            list\n"
       << "Aliases:         firmwares\n\n"
       << "Description:\n"
       << "Prints a list of all availalbe firmwares. The identifier has\n"
       << "to be used for other commands."
       << std::endl;
}

/* }}} */
/* InfoCommand {{{ */

InfoCommand::InfoCommand(Firmwarepool *firmwarepool)
    : AbstractCommand("info"), m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool InfoCommand::execute(CommandArgVector   args,
                          core::StringVector options,
                          std::ostream       &os)
{
    std::string fwstr = args[0]->getString();
    Firmware *fw = m_firmwarepool->getFirmware(fwstr);
    if (!fw)
        throw core::ApplicationError(fwstr + ": Invalid firmware specified.");

    os << "Identifier   : " << fw->getName() << std::endl;
    os << "Name         : " << fw->getLabel() << std::endl;
    os << "URL          : " << fw->getUrl() << std::endl;
    os << "File name    : " << fw->getFilename() << std::endl;
    os << "Author       : " << fw->getAuthor() << std::endl;
    os << "Version      : " << fw->formatDateVersion() << std::endl;
    if (fw->getMD5Sum().size() > 0)
        os << "MD5sum       : " << fw->getMD5Sum() << std::endl;

    // vendor ID and/or Product ID
    if (fw->updateDevice().isValid())
        os << "Device ID(s) : " << fw->updateDevice().formatDeviceId() << std::endl;

    os << std::endl;
    os << "Description" << std::endl;
    os << core::wordwrap(fw->getDescription(), DEFAULT_TERMINAL_WIDTH) << std::endl;

    if (!CliConfiguration::config().getBatchMode()) {
        os << std::endl;
        os << "For information about the Pin assignment, use the "
           << "\"pin " << fw->getName() << "\" command." << std::endl;
    }

    // reset fill character
    os << std::setfill(' ');

    return true;
}

/* -------------------------------------------------------------------------- */
size_t InfoCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type InfoCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string InfoCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "firmware";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector InfoCommand::aliases() const
{
    core::StringVector ret;
    ret.push_back("firmware");
    return ret;
}

/* -------------------------------------------------------------------------- */
core::StringVector InfoCommand::getCompletions(const std::string &start,
                                               size_t            pos,
                                               bool              option,
                                               bool              *filecompletion) const
{
    if (pos != 0 || option)
        return core::StringVector();

    return complete_firmware(start, m_firmwarepool);
}

/* -------------------------------------------------------------------------- */
std::string InfoCommand::help() const
{
    return "Prints information about a specific firmware.";
}

/* -------------------------------------------------------------------------- */
void InfoCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            info\n"
       << "Aliases:         firmware\n\n"
       << "Description:\n"
       << "Displays information about a specific firmware. To obtain a list\n"
       << "of all available firmwares, use the \"list\" command. To get\n"
       << "information about pin assignment, use \"pin <firmware>\"."
       << std::endl;
}

/* }}} */
/* PinCommand {{{ */
PinCommand::PinCommand(Firmwarepool *firmwarepool)
    : AbstractCommand("pin"), m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool PinCommand::execute(CommandArgVector   args,
                         core::StringVector options,
                         std::ostream       &os)
{
    std::string fwstr = args[0]->getString();
    Firmware *fw = m_firmwarepool->getFirmware(fwstr);
    if (!fw)
        throw core::ApplicationError(fwstr + ": Invalid firmware specified.");

    if (!CliConfiguration::config().getBatchMode()) {
        os << "            +----------------+" << std::endl;
        os << "            |  9  7  5  3  1 |" << std::endl;
        os << "            | 10  8  6  4  2 |" << std::endl;
        os << "            +----------------+" << std::endl;
        os << std::endl;
    }

    // calc max length
    size_t maxlen = 0;
    core::StringVector pins = fw->getPins();
    for (core::StringVector::const_iterator it = pins.begin();
            it != pins.end(); ++it)
        maxlen = std::max(maxlen, fw->getPin(*it).size());
    maxlen += 5;

    for (int i = 1; i <= 10; i += 2) {
        std::stringstream name1, name2;
        name1 << "P" << i;
        name2 << "P" << i+1;

        os << "[" << std::right << std::setw(5) << name1.str() << "] "
             << std::setw(maxlen) << std::left << fw->getPin(name1.str());
        os << "[" << std::right << std::setw(5) << name2.str() << "] "
             << std::left << fw->getPin(name2.str()) << std::endl;

        core::StringVector::iterator delit = find(pins.begin(), pins.end(), name1.str());
        if (delit != pins.end())
            pins.erase(delit);
        delit = find(pins.begin(), pins.end(), name2.str());
        if (delit != pins.end())
            pins.erase(delit);
    }

    for (unsigned int i = 0; i < pins.size(); i += 2) {
        os << "[" << std::right << std::setw(5) << pins[i] << "] "
             << std::setw(maxlen) << std::left << fw->getPin(pins[i]);

        if (i + 1 < pins.size())
            os << "[" << std::right << std::setw(5) << pins[i+1] << "] "
                << std::left << fw->getPin(pins[i+1]);
        os << std::endl;
    }

    return true;
}

/* -------------------------------------------------------------------------- */
size_t PinCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type PinCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string PinCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "firmware";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector PinCommand::aliases() const
{
    core::StringVector ret;
    ret.push_back("pins");
    return ret;
}

/* -------------------------------------------------------------------------- */
core::StringVector PinCommand::getCompletions(const std::string &start,
                                              size_t            pos,
                                              bool              option,
                                              bool              *filecompletion) const
{
    if (pos != 0 || option)
        return core::StringVector();

    return complete_firmware(start, m_firmwarepool);
}

/* -------------------------------------------------------------------------- */
std::string PinCommand::help() const
{
    return "Prints information about pin assignment.";
}

/* -------------------------------------------------------------------------- */
void PinCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            pin\n"
       << "Aliases:         pins\n"
       << "Argument:        firmware\n\n"
       << "Description:\n"
       << "Prints a list about pin usage. This might help you when connecting\n"
       << "something to your USBprog."
       << std::endl;
}

/* }}} */
/* DownloadCommand {{{ */

/* -------------------------------------------------------------------------- */
DownloadCommand::DownloadCommand(Firmwarepool *firmwarepool)
    : AbstractCommand("download"), m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool DownloadCommand::downloadAll(std::ostream &os)
{
    std::vector<Firmware *> firmwares = m_firmwarepool->getFirmwareList();

    for (std::vector<Firmware *>::const_iterator it = firmwares.begin();
            it != firmwares.end(); ++it) {
        try {
            if (m_firmwarepool->isFirmwareOnDisk((*it)->getName()))
                os << "Firmware " << (*it)->getLabel() << " is already there."
                   << std::endl;
            else {
                os << "Downloading " << (*it)->getLabel() << " ..." << std::endl;
                m_firmwarepool->downloadFirmware((*it)->getName());
            }
        } catch (const std::exception &ex) {
            os << "Error while downloading firmware " + (*it)->getName() +
                ": " + ex.what() << std::endl;
        }
    }

    return true;
}

/* -------------------------------------------------------------------------- */
bool DownloadCommand::execute(CommandArgVector   args,
                              core::StringVector options,
                              std::ostream       &os)
{
    std::string fwstr = args[0]->getString();
    if (CliConfiguration::config().isOffline()) {
        os << "Software is in offline mode. Downloading is not possbile."
           << std::endl;
        return true;
    }

    if (fwstr == "all")
        return downloadAll(os);

    Firmware *fw = m_firmwarepool->getFirmware(fwstr);
    if (!fw)
        throw core::ApplicationError(fwstr + ": Invalid firmware specified.");

    try {
        m_firmwarepool->downloadFirmware(fwstr);
        os << "Firmware " + fw->getName() + " has been downloaded successfully."
           << std::endl;
    } catch (const std::exception &ex) {
        os << "Error while downloading firmware: " << ex.what() << std::endl;
    }

    return true;
}

/* -------------------------------------------------------------------------- */
size_t DownloadCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type DownloadCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string DownloadCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "firmware";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector DownloadCommand::aliases() const
{
    core::StringVector ret;
    ret.push_back("get");
    return ret;
}

/* -------------------------------------------------------------------------- */
core::StringVector DownloadCommand::getCompletions(
        const std::string &start, size_t pos, bool option, bool *filecompletion) const
{
    if (pos != 0 || option)
        return core::StringVector();

    core::StringVector comp =  complete_firmware(start, m_firmwarepool);
    if (core::str_starts_with("all", start))
        comp.push_back("all");
    return comp;
}

/* -------------------------------------------------------------------------- */
std::string DownloadCommand::help() const
{
    return "Downloads a firmware file.";
}

/* -------------------------------------------------------------------------- */
void DownloadCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            download\n"
       << "Argument:        firmware\n\n"
       << "Description:\n"
       << "Downloads the specified firmware from the internet. Only available\n"
       << "when USBprog is not in offline mode. Use \"download all\" to download\n"
       << "all available firmware files."
       << std::endl;
}

/* }}} */
/* CacheCommand {{{ */

/* -------------------------------------------------------------------------- */
CacheCommand::CacheCommand(Firmwarepool *firmwarepool)
    : AbstractCommand("cache"), m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool CacheCommand::execute(CommandArgVector   args,
                           core::StringVector options,
                           std::ostream       &os)
{
    std::string cmd = args[0]->getString();

    try {
        if (cmd == "clean")
            m_firmwarepool->cleanCache();
        else if (cmd == "delete")
            m_firmwarepool->deleteCache();
        else
            throw core::ApplicationError(cmd + ": Invalid command for \"cache\".");
    } catch (const core::IOError &ioe) {
        throw core::ApplicationError(std::string("I/O error: ") + ioe.what());
    }

    return true;
}

/* -------------------------------------------------------------------------- */
size_t CacheCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type CacheCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string CacheCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "operation [clean/delete]";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector CacheCommand::getCompletions(
        const std::string &start, size_t pos, bool option, bool *filecompletion) const
{
    if (pos != 0 || option)
        return core::StringVector();

    core::StringVector result;
    if (core::str_starts_with("clean", start))
        result.push_back("clean");
    if (core::str_starts_with("delete", start))
        result.push_back("delete");

    return result;
}

/* -------------------------------------------------------------------------- */
std::string CacheCommand::help() const
{
    return "Performs operation on the cache.";
}

/* -------------------------------------------------------------------------- */
void CacheCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            cache\n"
       << "Argument:        operation (clean/delete)\n\n"
       << "Description:\n"
       << "The \"delete\" operation deletes the whole cache. All firmware files\n"
       << "have to be downloaded again. The \"clean\" operation only deletes\n"
       << "obsolete firmware files, i.e. firmware data for which a newer version\n"
       << "is available."
       << std::endl;
}

/* }}} */
/* DevicesCommand {{{ */

/* -------------------------------------------------------------------------- */
DevicesCommand::DevicesCommand(core::DeviceManager *deviceManager,
                               Firmwarepool        *firmwarepool)
    : AbstractCommand("devices")
    , m_deviceManager(deviceManager)
    , m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool DevicesCommand::execute(CommandArgVector   args,
                             core::StringVector options,
                             std::ostream       &os)
{
    try {
        m_deviceManager->discoverUpdateDevices(m_firmwarepool->getUpdateDeviceList());
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string(err.what()));
    }

    if (m_deviceManager->getNumberUpdateDevices() == 0)
        os << "No devices found." << std::endl;
    else
        m_deviceManager->printDevices(os);

    if (!CliConfiguration::config().getBatchMode() &&
            m_deviceManager->getNumberUpdateDevices() > 1)
        os << std::endl
           << "       * = Currently selected update device." << std::endl;

    return true;
}

/* -------------------------------------------------------------------------- */
std::string DevicesCommand::help() const
{
    return "Lists all update devices.";
}

/* -------------------------------------------------------------------------- */
void DevicesCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            devices\n\n"
       << "Description:\n"
       << "Lists all available update devices."
       << std::endl;
}

/* }}} */
/* DeviceCommand {{{ */

/* -------------------------------------------------------------------------- */
DeviceCommand::DeviceCommand(core::DeviceManager *deviceManager,
                             Firmwarepool        *firmwarepool)
    : AbstractCommand("device")
    , m_deviceManager(deviceManager)
    , m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool DeviceCommand::execute(CommandArgVector   args,
                            core::StringVector options,
                            std::ostream       &os)
{
    std::string device = args[0]->getString();

    try {
        if (m_deviceManager->getNumberUpdateDevices() == 0)
            m_deviceManager->discoverUpdateDevices(m_firmwarepool->getUpdateDeviceList());
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string(err.what()));
    }

    bool is_number = true;
    for (unsigned int i = 0; i < device.size(); i++) {
        if (!isdigit(device[i])) {
            is_number = false;
            break;
        }
    }

    int updatedevice = -1;

    if (is_number) {
        std::stringstream ss;
        ss << device;
        ss >> updatedevice;

        int number_of_devices = m_deviceManager->getNumberUpdateDevices();
        if (updatedevice < 0 || updatedevice >= number_of_devices)
            throw core::ApplicationError("Invalid device number specified.");
    } else {

        for (unsigned int i = 0; i < m_deviceManager->getNumberUpdateDevices(); i++) {
            core::Device *dev = m_deviceManager->getDevice(i);

            if (dev->getShortName() == device) {
                updatedevice = i;
                break;
            }
        }

        if (updatedevice == -1)
            throw core::ApplicationError("Invalid update device name specified.");
    }

    m_deviceManager->setCurrentUpdateDevice(updatedevice);

    return true;
}

/* -------------------------------------------------------------------------- */
size_t DeviceCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type DeviceCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string DeviceCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "device";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector DeviceCommand::getCompletions(const std::string &start,
                                                 size_t            pos,
                                                 bool              option,
                                                 bool              *filecompletion) const
{
    if (pos != 0 || option)
        return core::StringVector();

    core::StringVector result;
    for (unsigned int i = 0; i < m_deviceManager->getNumberUpdateDevices(); i++) {
        std::stringstream ss;
        ss << i;
        result.push_back(ss.str());
    }

    return result;
}


/* -------------------------------------------------------------------------- */
std::string DeviceCommand::help() const
{
    return "Sets the update device.";
}

/* -------------------------------------------------------------------------- */
void DeviceCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            cache\n"
       << "Argument:        device number|device name\n\n"
       << "Description:\n"
       << "Sets the update device for the \"upload\" command. You have to use\n"
       << "an integer number which you can obtain with the \"devices\" command.\n"
       << "Alternatively, you can also use the short device name in the 2nd line\n"
       << "of the output of the \"devices\" command\n"
       << std::endl;
}

/* }}} */
/* UploadCommand {{{ */

/* -------------------------------------------------------------------------- */
UploadCommand::UploadCommand(core::DeviceManager *deviceManager,
                             Firmwarepool        *firmwarepool)
    : AbstractCommand("upload")
    , m_deviceManager(deviceManager)
    , m_firmwarepool(firmwarepool)
{}

/* -------------------------------------------------------------------------- */
bool UploadCommand::execute(CommandArgVector   args,
                            core::StringVector options,
                            std::ostream       &os)
{
    std::string firmware = args[0]->getString();
    HashNotifier hn(DEFAULT_TERMINAL_WIDTH);

    try {
        if (m_deviceManager->getNumberUpdateDevices() == 0)
            m_deviceManager->discoverUpdateDevices();
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string(err.what()));
    }

    core::ByteVector data;

    if (core::Fileutil::isPathName(firmware)) {
        /* read from file */

        firmware = core::Fileutil::resolvePath(firmware);
        try {
            data = core::Fileutil::readBytesFromFile(firmware);
        } catch (const core::IOError &ioe) {
            throw core::ApplicationError(std::string("Error while reading data from file: ")+ioe.what());
        }
    } else {
        /* use pool */

        Firmware *fw = m_firmwarepool->getFirmware(firmware);
        if (!fw)
            throw core::ApplicationError(firmware+": Invalid firmware specified.");

        try {
            m_firmwarepool->fillFirmware(firmware);
        } catch (const core::IOError &err) {
            throw core::ApplicationError(std::string("I/O Error: ") + err.what());
        }

        data = fw->getData();
    }

    core::Device *dev = m_deviceManager->getCurrentUpdateDevice();
    if (!dev)
        throw core::ApplicationError("Unable to find update device.");

    // switch in update mode
    if (!dev->isUpdateMode()) {
        try {
            os << "Switching to update mode ..." << std::endl;
            m_deviceManager->switchUpdateMode();
        } catch (const core::IOError &err) {
            throw core::ApplicationError(std::string("I/O Error: ") + err.what());
        }
    }

    dev = m_deviceManager->getCurrentUpdateDevice();
    if (!dev)
        throw core::ApplicationError("Unable to find update device (2).");
    core::UsbprogUpdater updater(dev);

    if (!CliConfiguration::config().getBatchMode() && !CliConfiguration::config().getDebug())
        updater.setProgress(&hn);

    try {
        os << "Opening device ..." << std::endl;
        updater.updateOpen();
        os << "Writing firmware ..." << std::endl;
        updater.writeFirmware(data);
        if (options.size() == 0) {
            os << "Starting device ..." << std::endl;
            updater.startDevice();
        }
        updater.updateClose();
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string("I/O Error: ") + err.what());
    }

    os << "Detecting new USB devices ..." << std::endl;
    core::usbprog_sleep(2);
    try {
        m_deviceManager->discoverUpdateDevices();
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string(err.what()));
    }

    return true;
}

/* -------------------------------------------------------------------------- */
size_t UploadCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type UploadCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:         return CommandArg::STRING;
        default:        return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string UploadCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:         return "firmware";
        default:        return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector UploadCommand::getCompletions(const std::string &start,
                                                 size_t            pos,
                                                 bool              option,
                                                 bool              *filecompletion) const
{
    if (pos != 0)
        return core::StringVector();

    if (option) {
        core::StringVector ret;
        if (core::str_starts_with("-nostart", start))
            ret.push_back("-nostart");
        return ret;
    } else {
        if (start.size() > 0 && core::Fileutil::isPathName(start)) {
            if (filecompletion)
                *filecompletion = true;
            return core::StringVector();
        } else
            return complete_firmware(start, m_firmwarepool);
    }
}


/* -------------------------------------------------------------------------- */
std::string UploadCommand::help() const
{
    return "Uploads a new firmware.";
}

/* -------------------------------------------------------------------------- */
void UploadCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            upload\n"
       << "Option:          -nostart\n"
       << "Argument:        firmware|filename\n\n"
       << "Description:\n"
       << "Uploads a new firmware. The firmware identifier can be found with\n"
       << "the \"list\" command. Alternatively, you can just specify a filename.\n"
       << "If you have more than one USBprog device connected, use the \"devices\"\n"
       << "command to obtain a list of available update devices and select one\n"
       << "with the \"device\" command."
       << std::endl;
}

/* -------------------------------------------------------------------------- */
core::StringVector UploadCommand::getSupportedOptions() const
{
    core::StringVector sv;
    sv.push_back("-nostart");
    return sv;
}

/* }}} */
/* StartCommand {{{ */

/* -------------------------------------------------------------------------- */
StartCommand::StartCommand(core::DeviceManager *deviceManager)
    : AbstractCommand("start")
    , m_deviceManager(deviceManager)
{}

/* -------------------------------------------------------------------------- */
bool StartCommand::execute(CommandArgVector     args,
                           core::StringVector   options,
                           std::ostream         &os)
{
    core::Device *dev = m_deviceManager->getCurrentUpdateDevice();
    if (!dev)
        throw core::ApplicationError("Unable to find update device.");
    core::UsbprogUpdater updater(dev);
    HashNotifier hn(DEFAULT_TERMINAL_WIDTH);

    if (!CliConfiguration::config().getBatchMode() && !CliConfiguration::config().getDebug())
        updater.setProgress(&hn);

    try {
        updater.updateOpen();
        updater.startDevice();
        os << "Device successfully started." << std::endl;
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string("I/O Error: ") + err.what());
    }

    return true;
}

/* -------------------------------------------------------------------------- */
std::string StartCommand::help() const
{
    return "Starts the firmware.";
}

/* -------------------------------------------------------------------------- */
void StartCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            start\n\n"
       << "Description:\n"
       << "Starts the currently uploaded firmware on the current update device."
       << std::endl;
}

/* }}} */
/* ResetCommand {{{ */

/* -------------------------------------------------------------------------- */
ResetCommand::ResetCommand(core::DeviceManager *deviceManager)
    : AbstractCommand("reset")
    , m_deviceManager(deviceManager)
{}

/* -------------------------------------------------------------------------- */
bool ResetCommand::execute(CommandArgVector     args,
                           core::StringVector   options,
                           std::ostream         &os)
{
    core::Device *dev = m_deviceManager->getCurrentUpdateDevice();
    if (!dev)
        throw core::ApplicationError("Unable to find update device.");
    core::UsbprogUpdater updater(dev);
    HashNotifier hn(DEFAULT_TERMINAL_WIDTH);

    if (!CliConfiguration::config().getBatchMode() && !CliConfiguration::config().getDebug())
        updater.setProgress(&hn);

    try {
        updater.updateOpen();
        updater.resetDevice();
        os << "Device successfully reset." << std::endl;
    } catch (const core::IOError &err) {
        throw core::ApplicationError(std::string("I/O Error: ") + err.what());
    }

    return true;
}

/* -------------------------------------------------------------------------- */
std::string ResetCommand::help() const
{
    return "Resets the firmware.";
}

/* -------------------------------------------------------------------------- */
void ResetCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            reset\n\n"
       << "Description:\n"
       << "Resets the given device. This is almost the same as unplugging and re-plugging\n"
       << "the device. However, it doesn't have any effect with USBprog devices currently."
       << std::endl;
}

/* }}} */
/* CopyingCommand {{{ */

/* -------------------------------------------------------------------------- */
CopyingCommand::CopyingCommand()
    : AbstractCommand("copying")
{}

/* -------------------------------------------------------------------------- */
bool CopyingCommand::execute(CommandArgVector   args,
                             core::StringVector options,
                             std::ostream       &os)
{
    os << "USBprog " << USBPROG_VERSION_STRING << std::endl;
    os << "Copyright (c) 2007, 2008 Bernhard Walle <bernhard@bwalle.de>\n\n";
    os << "This program is free software: you can redistribute it and/or modify\n"
       << "it under the terms of the GNU General Public License as published by\n"
       << "the Free Software Foundation, either version 2 of the License, or\n"
       << "(at your option) any later version.\n\n"
       << "This program is distributed in the hope that it will be useful,\n"
       << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
       << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
       << "GNU General Public License for more details.\n\n"
       << "You should have received a copy of the GNU General Public License\n"
       << "along with this program. If not, see <http://www.gnu.org/licenses/>.\n";

   return true;
}

/* -------------------------------------------------------------------------- */
std::string CopyingCommand::help() const
{
    return "Displays the copyright";
}

/* -------------------------------------------------------------------------- */
core::StringVector CopyingCommand::aliases() const
{
    core::StringVector ret;
    ret.push_back("license");
    return ret;
}

/* -------------------------------------------------------------------------- */
void CopyingCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            copying\n"
       << "Aliases:         license\n\n"
       << "Description:\n"
       << "Shows the license of the program."
       << std::endl;
}

/* }}} */

} // end namespace cli
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et:
