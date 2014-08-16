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
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "serialfile.h"
#include "serialfile_private_posix.h"

namespace bw {
namespace io {

/* SerialFile {{{ */

SerialFile::SerialFile(const std::string &portName)
    : d( new SerialFilePrivate(portName) )
{}

SerialFile::~SerialFile()
{
    closePort();
    delete d;
}

bool SerialFile::openPort()
{
    if (!createLock()) {
        d->lastError = "Device is locked.";
        return false;
    }

    d->fd = open(d->fileName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (d->fd < 0) {
        d->lastError = std::string(std::strerror(errno));
        removeLock();
        return false;
    }

    return true;
}

void SerialFile::closePort()
{
    if (d->fd == -1)
        return;

    close(d->fd);
    d->fd = -1;
    removeLock();
}

SerialFile &SerialFile::operator<<(const std::string& str)
{
    errno = 0;
    int ret = write(d->fd, str.c_str(), str.length());
    if (ret != int(str.length())) {
        d->lastError = std::string(std::strerror(errno));
        throw IOError(getLastError());
    }

    return *this;
}

SerialFile &SerialFile::operator<<(char c)
{
    return operator<<( std::string(1, c) );
}

SerialFile &SerialFile::operator>>(std::string& str)
{
    char buffer[BUFSIZ];

    int ret = read(d->fd, buffer, BUFSIZ);
    if (ret < 0) {
        d->lastError = std::string(std::strerror(errno));
        throw IOError(getLastError());
    }

    str = std::string(buffer, ret);

    return *this;
}

std::string SerialFile::readLine()
{
    std::string result;
    char lastCharacter;

    do {
        int ret = read(d->fd, &lastCharacter, 1);
        if (ret < 0) {
            d->lastError = std::string(std::strerror(errno));
            throw IOError(getLastError());
        }

        if (lastCharacter != '\r' && lastCharacter != '\n')
            result += lastCharacter;

    } while (lastCharacter != '\n');

    return result;
}

std::string SerialFile::getLastError() const
{
    return d->lastError;
}

static bool int_to_speed(int baudrate, speed_t &speed)
{
    switch (baudrate) {
        case 0:
            speed = B0;
            break;
        case 50:
            speed = B50;
            break;
        case 75:
            speed = B75;
            break;
        case 110:
            speed = B110;
            break;
        case 134:
            speed = B134;
            break;
        case 150:
            speed = B150;
            break;
        case 200:
            speed = B200;
            break;
        case 300:
            speed = B300;
            break;
        case 600:
            speed = B600;
            break;
        case 1200:
            speed = B1200;
            break;
        case 1800:
            speed = B1800;
            break;
        case 2400:
            speed = B2400;
            break;
        case 4800:
            speed = B4800;
            break;
        case 9600:
            speed = B9600;
            break;
        case 19200:
            speed = B19200;
            break;
        case 38400:
            speed = B38400;
            break;
        case 57600:
            speed = B57600;
            break;
        case 115200:
            speed = B115200;
            break;
        case 230400:
            speed = B230400;
            break;
        default:
            return false;
    }

    return true;
}

bool SerialFile::reconfigure(int            baudrate,
                             FlowControl    flowControl,
                             bool           rawMode)
{
    fcntl(d->fd, F_SETFL, 0);
    struct termios options;

    tcgetattr(d->fd, &options);

    // set the baudrate
    speed_t speed;
    if (!int_to_speed(baudrate, speed)) {
        d->lastError = "Unsupported baudrate.";
        return false;
    }
    cfsetispeed(&options, speed);
    cfsetospeed(&options, speed);

    options.c_cflag |= (CLOCAL | CREAD);

    // raw mode
    if (rawMode) {
        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
        options.c_oflag &= ~OPOST;
    }

    // Disable translation of \r to \n since \r\n results in \n\n which doesn't make
    // sense. The input has to deal with \r\n line endings and readLine() is able to do
    // that.
    options.c_iflag &= ~ICRNL;

    // initialize all control characters
    // default values can be found in /usr/include/termios.h, and are given
    // in the comments, but we don't need them here
    options.c_cc[VINTR]    = 0;     // Ctrl-c
    options.c_cc[VQUIT]    = 0;     // Ctrl-backslash
    options.c_cc[VERASE]   = 0;     // del
    options.c_cc[VKILL]    = 0;     // @
    options.c_cc[VEOF]     = 4;     // Ctrl-d
    options.c_cc[VTIME]    = 0;     // inter-character timer unused
    options.c_cc[VMIN]     = 1;     // blocking read until 1 character arrives
#if defined(VSWTC)
    options.c_cc[VSWTC]    = 0;     // '\0'
#elif defined(VSWTCH)
    options.c_cc[VSWTCH]   = 0;     // '\0'
#endif
    options.c_cc[VSTART]   = 0;     // Ctrl-q
    options.c_cc[VSTOP]    = 0;     // Ctrl-s
    options.c_cc[VSUSP]    = 0;     // Ctrl-z
    options.c_cc[VEOL]     = 0;     // '\0'
    options.c_cc[VREPRINT] = 0;     // Ctrl-r
    options.c_cc[VDISCARD] = 0;     // Ctrl-u
    options.c_cc[VWERASE]  = 0;     // Ctrl-w
    options.c_cc[VLNEXT]   = 0;     // Ctrl-v
    options.c_cc[VEOL2]    = 0;     // '\0'

    // flow control
    switch (flowControl)
    {
        case FC_HARDWARE:
            options.c_cflag |= CRTSCTS;
            break;
        case FC_XON_XOFF:
            options.c_iflag |= (IXON | IXOFF | IXANY);
            break;
        default:
            options.c_cflag &= ~CRTSCTS;
            options.c_iflag &= ~(IXON | IXOFF | IXANY);
            break;
    }

    // set 8N1
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    tcsetattr(d->fd, TCSANOW, &options);

    return true;
}

std::string SerialFile::str() const
{
    return d->fileName;
}

/* }}} */

} // end namespace io
} // end namespace bw


std::ostream &operator<<(std::ostream &os, const bw::io::SerialFile &serialFile)
{
    return os << serialFile.str();
}
