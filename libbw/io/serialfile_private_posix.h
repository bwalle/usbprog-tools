/* {{{
 * Copyright (c) 2011, Bernhard Walle <bernhard@bwalle.de>
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

#ifndef SERIALFILE_PRIVATE_POSIX_H
#define SERIALFILE_PRIVATE_POSIX_H

#include <string>

#include "exithandler.h"

namespace bw {
namespace io {

/* SerialFilePrivate {{{ */

/**
 * \brief Data object for SerialFile.
 *
 * The reason why that data objects are not members of SerialFile is just that we can provide
 * the same interface for different platforms and have the concrete (typed) members as private
 * data of the platform implementation.
 *
 * The lock file name and exit handlers are only used on Linux.
 */
struct SerialFilePrivate
{
    SerialFilePrivate(const std::string &portName)
        : fileName(portName)
        , fd(-1)
        , exithandler(NULL)
    {}

    std::string fileName;
    std::string lastError;
    int         fd;
    std::string lockfile;
    ExitHandler *exithandler;
};

/* }}} */

} // end namespace io
} // end namespace bw

#endif /* SERIALFILE_PRIVATE_POSIX_H */
