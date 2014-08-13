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
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libbw/log/errorlog.h>
#include "tempfile.h"

namespace bw {
namespace io {

struct TempFilePrivate {
    TempFilePrivate()
        : fd(-1)
    {}

    int fd;
};

std::string TempFile::_create(const std::string &namepart)
{
    d = new TempFilePrivate;

    std::string nametemplate("/tmp");
    if (getenv("TMPDIR") && std::strlen("TMPDIR") > 0) {
        std::string dir(getenv("TMPDIR"));
        struct stat statresult;
        int ret = stat(dir.c_str(), &statresult);
        if (ret < 0)
            BW_ERROR_ERR("Invalid value of TMPDIR ('%s'): Directory does not exist", dir.c_str());
        else if (!S_ISDIR(statresult.st_mode))
            BW_ERROR_ERR("Invalid value of TMPDIR ('%s'): Not a directory", dir.c_str());
        else
            nametemplate = dir;
    }

    if (nametemplate[nametemplate.size()-1] != '/')
        nametemplate += "/" + namepart;
    nametemplate += ".XXXXXX";

    char *nametemplateMod = strdup(nametemplate.c_str());
    int ret = mkstemp(nametemplateMod);
    nametemplate = nametemplateMod;
    std::free(nametemplateMod);

    if (ret < 0)
        throw SystemIOError("Unable to create temporary file '" + nametemplate + " '", errno);

    d->fd = ret;
    return nametemplate;
}

uint64_t TempFile::nativeHandle() const
{
    return d ? d->fd : -1;
}

void TempFile::_close()
{
    ::close(d->fd);
    delete d;
}

} // end namespace io
} // end namespace bw
