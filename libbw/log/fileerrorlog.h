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
#ifndef LIBBW_LOG_FILEERRORLOG_H_
#define LIBBW_LOG_FILEERRORLOG_H_

#include <cstdio>
#include <string>

#include "errorlog.h"
#include "bwconfig.h"
#ifdef HAVE_THREADS
#  include <thread/mutex.h>
#endif

namespace bw {

/* FileErrorLog {{{ */

/**
 * \brief Error log implementation for std::FILE
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup log
 */
class FileErrorlog : public Errorlog {

public:
    /// Let Errorlog create instances of FileErrorlog, and only Errorlog.
    friend class Errorlog;

protected:
    /**
     * \brief Creates a new FileErrorlog.
     *
     * Don't use that function directly. Instead, use Errorlog::configure().
     *
     * \param[in] filename the name of the file to which the logger should log. The
     *            special values \c "stderr" and \c "stdout" are supported.
     */
    FileErrorlog(const char *filename="stderr");

    /**
     * \brief Destructor
     */
    ~FileErrorlog();

    /**
     * \copydoc Errorlog::vlog()
     */
    void vlog(Errorlog::Level level, const char *msg, std::va_list args);

private:
    std::FILE *m_file;
    bool m_closeInDtor;
#ifdef HAVE_THREADS
    thread::Mutex m_mutex;
#endif
};

/* }}} */

} // end namespace bw

#endif /* LIBBW_LOG_FILEERRORLOG_H_ */

// vim: set sw=4 ts=4 et fdm=marker:
