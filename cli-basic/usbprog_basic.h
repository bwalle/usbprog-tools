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
#ifndef USBPROG_BASIC_H
#define USBPROG_BASIC_H

#include <string>

#include <usbprog-core/error.h>

namespace usbprog {
namespace cli_basic {

/* Action {{{ */

enum Action {
    ACTION_ERROR = -1,
    ACTION_PRINT_HELP,
    ACTION_LIST_DEVICES,
    ACTION_UPLOAD_FIRMWARE
};

/* }}} */
/* ErrorCode {{{ */

enum ErrorCode {
    RC_SUCCESS = 0,
    RC_DEV_NOT_FOUND = 1,
    RC_IOERROR,
    RC_FILE_NOT_EXIST,
    RC_INVALID_COMMANDLINE,
    RC_OTHER_ERROR = 255
};

/* }}} */
/* UsbprogBasic {{{ */

class UsbprogBasic {

public:
    UsbprogBasic(int argc, char *argv[]);
    virtual ~UsbprogBasic();

public:
    int exec();

protected:
    Action parseCommandLine(int &deviceNumber, std::string &fw) const;
    void printHelp() const;
    ErrorCode listDevices() const;
    ErrorCode uploadFirmware(int                deviceNumber,
                             const std::string  &firmwareFile) const;

private:
    int m_argc;
    char **m_argv;
};

/* }}} */

} // end namespace cli
} // end namespace cli_basic

#endif /* COMMANDS_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
