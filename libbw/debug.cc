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
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>

#include "debug.h"

namespace bw {

/* ---------------------------------------------------------------------------------------------- */
Debug *Debug::m_instance = NULL;

/* ---------------------------------------------------------------------------------------------- */
Debug *Debug::debug()
{
    if (!m_instance)
        m_instance = new Debug();

    return m_instance;
}

/* ---------------------------------------------------------------------------------------------- */
Debug::Debug()
    : m_debuglevel(DL_NONE)
    , m_handle(stderr)
{}

/* ---------------------------------------------------------------------------------------------- */
void Debug::setLevel(Debug::Level level)
{
    m_debuglevel = level;
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::setFileHandle(FILE *handle)
{
    if (!handle)
        m_handle = stderr;
    else
        m_handle = handle;
}

/* ---------------------------------------------------------------------------------------------- */
FILE *Debug::getFileHandle() const
{
    return m_handle;
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::dbg(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_DEBUG, msg, valist);
    va_end(valist);
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::info(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_INFO, msg, valist);
    va_end(valist);
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::trace(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_TRACE, msg, valist);
    va_end(valist);
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::dbg(const std::string &string)
{
    return dbg(string.c_str());
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::info(const std::string &string)
{
    return info(string.c_str());
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::trace(const std::string &string)
{
    return trace(string.c_str());
}


/* ---------------------------------------------------------------------------------------------- */
void Debug::msg(Debug::Level level, const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(level, msg, valist);
    va_end(valist);
}

/* ---------------------------------------------------------------------------------------------- */
void Debug::vmsg(Debug::Level level, const char *msg, std::va_list args)
{
    // if the global debug level is too small, then just do nothing
    if (level < m_debuglevel)
        return;

    size_t len = strlen(msg) + 20;
    char *newmsg = new char[len];
    if (!newmsg)
        return;
    memset(newmsg, 0, len);

    // prepend dump level
    switch (level) {
        case DL_TRACE:
            strcat(newmsg, "TRACE: ");
            break;

        case DL_INFO:
            strcat(newmsg, "INFO: ");
            break;

        case DL_DEBUG:
            strcat(newmsg, "DEBUG: ");
            break;

        default:    // make the compiler happy
            break;
    }
    strcat(newmsg, msg);

    // append '\n' if there's no one at the end
    len = strlen(newmsg);
    if (newmsg[len-1] != '\n') {
        newmsg[len++] = '\n';
        newmsg[len] = '\0';
    }

    vfprintf(m_handle, newmsg, args);

    delete[] newmsg;
}

/* ---------------------------------------------------------------------------------------------- */
Debug::Level Debug::getLevel() const
{
    return m_debuglevel;
}

/* ---------------------------------------------------------------------------------------------- */
bool Debug::isDebugEnabled() const
{
    return m_debuglevel < DL_NONE;
}

} // end namespace bw

// :tabSize=4:indentSize=4:noTabs=true:mode=c++:folding=explicit:collapseFolds=1:maxLineLen=100:
