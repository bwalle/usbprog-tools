/* {{{
 * Copyright (c) 2008-2010, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */
#ifndef SERIALFILE_H
#define SERIALFILE_H

#include <string>
#include <vector>
#include <iostream>

#include "bwerror.h"

/**
 * @file serialfile.h libbw/serialfile.h
 * @brief Simplify communication with serial ports
 *
 * This file provides functionality to communicate with serial ports.
 * The abstraction is platform-independent, but the implementation is currently
 * only done for POSIX-compatible operating systems, tested only on Linux.
 */

namespace bw {

struct SerialFilePrivate;

/* SerialFile {{{ */

/**
 * @class SerialFile serialfile.h bwutil/serialfile.h
 *
 * @brief Communicate with serial ports
 *
 * This class provides some abstraction above the historic APIs of some operating systems
 * when it comes to communicate with serial port. We only want to send and receive data and
 * set the communication parameters. No control terminal etc. etc.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class SerialFile {

    public:
        friend std::ostream &operator<<(std::ostream &os, const SerialFile &serialFile);

    public:
        /**
         * Flow control.
         */
        enum FlowControl {
            FC_NONE,           /**< flow control disabled */
            FC_HARDWARE,       /**< hardware flow control */
            FC_XON_XOFF        /**< software flow control */
        };

        /**
         * @brief Creates a new SerialFile object
         *
         * Creates a new object but does not open the port. Use openPort() to open the port.
         *
         * @param[in] portName the name of the port. On POSIX this is the name of the
         *            device file that represents the port, for example <tt>/dev/ttyS0</tt>
         *            for the first "native" serial port or <tt>/dev/ttyUSB0</tt> for the first
         *            USB port in the system.
         */
        SerialFile(const std::string &portName);

        /**
         * @brief Destroys the object.
         *
         * Closes the port if necessary.
         */
        virtual ~SerialFile();

        /**
         * Opens the port.
         *
         * @return @c true on success, @c false on failure. Use getLastError() to get a
         *         human-readable description of the error cause.
         */
        bool openPort();

        /**
         * @brief Closes the port.
         *
         * It's valid to call this function multiple times.
         */
        void closePort();

        /**
         * @brief Outputs the string
         *
         * Example:
         *
         * @code
         * serialPort << std::string("bla");
         * @endcode
         *
         * @param[in] string the string that should be written to the serial port.
         * @return a reference to the serial port object
         */
        SerialFile &operator<<(const std::string& string)
        throw (IOError);

        /**
         * @brief Outputs a character.
         *
         * Example:
         *
         * @code
         * serialPort << 'x';
         * @endcode
         *
         * @param[in] c the character to write to the serial port.
         * @return a reference to the serial port.
         */
        SerialFile &operator<<(char c)
        throw (IOError);

        /**
         * @brief Outputs a integer in hexadecimal.
         *
         * Example:
         *
         * @code
         * serialPort << 0x25000;
         * @endcode
         *
         * @param[in] number the number to write to the serial port (as hexadecimal value).
         * @return a reference to the serial port.
         */
        SerialFile &operator<<(unsigned long number)
        throw (IOError);

        /**
         * @brief Reads string
         *
         * Example:
         *
         * @code
         * std::string line;
         * serialPort >> line;
         * @endcode
         *
         * @param[in] string the string in which the data is stored.
         * @return a reference to the serial port.
         */
        SerialFile &operator>>(std::string &string)
        throw (IOError);

        /**
         * @brief Returns the last error as string.
         *
         * @return the last error as string
         */
        std::string getLastError() const;

        /**
         * @brief Reconfigures the serial port.
         *
         * @param[in] baudrate the new baudrate
         * @param[in] flowControl the flow control setting
         * @param[in] rawMode @c true if the raw mode should be used (that is what you normally
         *            want if you're using the serial port just as communication interface),
         *            @c false if not.
         * @todo make the number of data bits configurable
         * @todo make the number of stop bits configurable
         *
         * @return @c true on success, @c false on failure. Use getLastError() to get a
         *         human-readable description of the error cause.
         */
        bool reconfigure(int            baudrate,
                         FlowControl    flowControl,
                         bool           rawMode = true);

    private:
        SerialFilePrivate *d;
};

/**
 * @brief Prints the port name to the given ostream
 *
 * Prints the name which has been passed in the constructor to the SerialFile object
 * to the given output stream.
 *
 * Use it for example for debugging:
 *
 * @code
 * SerialFile serialFile(portName);
 * if (!serialFile.openPort()) {
 *     std::cerr << "Unable to open serial device '" << serialFile << "': "
 *               << serialFile.getLastError() << std::endl;
 *     return false;
 * }
 * @endcode
 *
 * @param[in,out] os the output stream to which the name should be printed
 * @param[in] serialFile the serial file object
 * @return @p os
 */
std::ostream &operator<<(std::ostream &os, const SerialFile &serialFile);

/* }}} */

} // end namespace bw

#endif /* SERIALFILE_H */
