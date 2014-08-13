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

/* Module-static stuff (lock file deletion) {{{ */

/**
 * \brief Computes the name of the lock file from the portName
 *
 * \param[in] portName the full path to the port that should be opened, e.g.
 *            /dev/ttyS0.
 * \return the full path to the lock file name and an empty string if
 *         no lock file should be created for \p portName
 */
static std::string computeLockFileName(const std::string &portName)
{
    char *absolutePort = ::realpath(portName.c_str(), NULL);
    if (!absolutePort)
        return std::string();

    std::string absolutePortString(absolutePort);
    std::free(absolutePort);
    std::string ret;

    if (absolutePortString.size() > 5 && absolutePortString.substr(0, 5) == "/dev/")
        ret = "/var/lock/LCK.." + absolutePortString.substr(5, absolutePortString.size());

    return ret;
}

/* }}} */
/* SerialFile {{{ */

/* ---------------------------------------------------------------------------------------------- */
bool SerialFile::createLock()
{
    d->lockfile = computeLockFileName(d->fileName);

    // if we don't need locking, everything is sane
    if (d->lockfile.empty())
        return true;

    // if the lock file is present, we failed to lock
    if (::access(d->lockfile.c_str(), F_OK) == 0)
        return false;

    // the race condition here is not important because the lock file is just
    // there to warn the user that another process is using the port, it's not
    // about security

    // now we can create the lock
    std::ofstream lock(d->lockfile.c_str());
    if (lock)
        lock << ::getpid() << std::endl;
    d->exithandler = new FileDeleteExitHandler(d->lockfile);
    registerExitHandler(d->exithandler);

    return true;
}

/* ---------------------------------------------------------------------------------------------- */
void SerialFile::removeLock()
{
    if (d->lockfile.empty())
        return;

    std::remove(d->lockfile.c_str());
    d->lockfile = "";
    unregisterExitHandler(d->exithandler);
    d->exithandler = NULL;
}

/* }}} */

} // end namespace io
} // end namespace bw
