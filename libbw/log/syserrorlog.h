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
#ifndef LIBBW_LOG_SYSERRORLOG_H_
#define LIBBW_LOG_SYSERRORLOG_H_

#include <cstdio>
#include <string>

#include "errorlog.h"

namespace bw {

/* SysErrorlog {{{ */

/**
 * \brief Error log implementation for syslog
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup log
 */
class SysErrorlog : public Errorlog {

public:
    /// Let Errorlog create instances of SysErrorlog, and only Errorlog.
    friend class Errorlog;

protected:
    /**
     * \brief Creates a new SysErrorlog.
     *
     * Don't use that function directly. Instead, use Errorlog::configure().
     *
     * \param[in] ident the syslog ident string
     */
    SysErrorlog(const char *ident="");

    /**
     * \brief Destructor
     */
    ~SysErrorlog();

    /**
     * \copydoc Errorlog::vlog()
     */
    void vlog(Errorlog::Level level, const char *msg, std::va_list args);

private:
    /**
     * \brief Converts a bw loglevel to syslog
     *
     * \param[in] level the log level
     * \return the syslog constant
     */
    int logToSyslog(Errorlog::Level level);
};

/* }}} */

} // end namespace bw

#endif /* LIBBW_LOG_SYSERRORLOG_H_ */
