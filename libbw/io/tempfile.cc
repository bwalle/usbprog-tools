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

#include <cerrno>
#include <cstdio>

#include <libbw/log/errorlog.h>
#include "tempfile.h"

namespace bw {
namespace io {

TempFile::TempFile(const std::string &namepart, Flags flags)
    : m_flags(flags)
    , m_open(false)
    , m_exitHandler(NULL)
    , d(NULL)
{
    m_name = _create(namepart);
    m_open = true;

    if (getenv("LIBBW_TEMPFILE_NODELETE"))
        m_flags = NoFlags;

    if (m_flags & DeleteOnExit) {
        m_exitHandler = new FileDeleteExitHandler(m_name);
        registerExitHandler(m_exitHandler);
    }
}

TempFile::~TempFile()
{
    close();
}

TempFile::Flags TempFile::flags() const
{
    return m_flags;
}

std::string TempFile::name() const
{
    return m_name;
}

void TempFile::close()
{
    if (!m_open)
        return;

    _close();
    m_open = false;

    if (m_flags & DeleteOnClose) {
        int ret = std::remove(m_name.c_str());
        if (ret != 0)
            BW_ERROR_WARNING("Unable to remove '%s': %s", m_name.c_str(), std::strerror(errno));
    }

    if (m_exitHandler) {
        unregisterExitHandler(m_exitHandler);
        m_exitHandler = NULL;
    }
}

} // end namespace io
} // end namespace bw
