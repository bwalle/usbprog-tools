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

#include <list>
#include <algorithm>
#include <cstdio>
#include <cerrno>

#include "exithandler.h"

#include "log/errorlog.h"

namespace bw {

/* FileDeleteExitHandler {{{ */

FileDeleteExitHandler::FileDeleteExitHandler(const std::string &filename)
    : m_filename(filename)
{}

void FileDeleteExitHandler::exitCleanup()
{
    std::remove(m_filename.c_str());
}

/* }}} */
/* registerExitHandler {{{ */

static std::list<ExitHandler *> s_exitHandlers;
static bool s_globalExitHandlerRegistered = false;

static void libbw_exithandler()
{
    std::list<ExitHandler *>::const_iterator it;
    for (it = s_exitHandlers.begin(); it != s_exitHandlers.end(); ++it) {
        ExitHandler *current = *it;
        current->exitCleanup();
        delete current;
    }
}

void registerExitHandler(ExitHandler *exitHandler)
{
    if (!s_globalExitHandlerRegistered) {
        if (std::atexit(libbw_exithandler) != 0)
            BW_ERROR_WARNING("Unable to register 'libbw_exithandler': %s", std::strerror(errno));
        s_globalExitHandlerRegistered = true;
    }

    s_exitHandlers.push_back(exitHandler);
}

void unregisterExitHandler(ExitHandler *exitHandler)
{
    std::list<ExitHandler *>::iterator it;
    it = std::find(s_exitHandlers.begin(), s_exitHandlers.end(), exitHandler);
    if (it != s_exitHandlers.end())
        s_exitHandlers.erase(it);
}

/* }}} */

} // namespace bw
