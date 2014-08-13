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
#include <cstdarg>

#include "bwconfig.h"
#include "errorlog.h"
#include "fileerrorlog.h"
#ifdef HAVE_SYSLOG
#  include "syserrorlog.h"
#endif

namespace bw {

/* Errorlog {{{ */

Errorlog *Errorlog::m_instance;

const char *Errorlog::levelToString(enum Errorlog::Level level)
{
    switch (level) {
        case LS_EMERG:
            return "EMERG";
        case LS_ALERT:
            return "ALERT";
        case LS_CRIT:
            return "CRITICAL";
        case LS_ERR:
            return "ERROR";
        case LS_WARNING:
            return "WARNING";
        default:
            return NULL;
    }
}

bool Errorlog::configure(enum LogMethod method, const char *option)
{
    if (m_instance) {
        delete m_instance;
        m_instance = NULL;
    }

    switch (method) {
    case LM_FILE:
        m_instance = new FileErrorlog(option);
        break;

#ifdef HAVE_SYSLOG
    case LM_SYSLOG:
        m_instance = new SysErrorlog(option);
        break;
#endif
    }

    return m_instance != NULL;
}

Errorlog *Errorlog::instance()
{
    return m_instance;
}

void Errorlog::emerg(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(LS_EMERG, msg, valist);
    va_end(valist);
}

void Errorlog::alert(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(LS_ALERT, msg, valist);
    va_end(valist);
}

void Errorlog::crit(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(LS_CRIT, msg, valist);
    va_end(valist);
}

void Errorlog::err(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(LS_ERR, msg, valist);
    va_end(valist);
}

void Errorlog::warning(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(LS_WARNING, msg, valist);
    va_end(valist);
}

void Errorlog::log(Errorlog::Level level, const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vlog(level, msg, valist);
    va_end(valist);
}

/* }}} */

} // end namespace bw
