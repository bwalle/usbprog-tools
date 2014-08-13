/* {{{
 * Copyright (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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

#include <libbw/stringutil.h>

#include "bwconfig.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#endif
#ifdef HAVE_DIRECT_H
#  include <direct.h>
#endif
#ifdef HAVE_GETPWUID_R
#  include <pwd.h>
#  include <unistd.h>
#endif
#ifdef _WIN32
#  include <windows.h>
#  include <shlobj.h>
#endif

#include "fileutils.h"

//
// S_ISDIR    {{{
//

#ifndef S_ISDIR
#  ifdef _S_IFDIR
#    define S_ISDIR(m) ((m) & _S_IFDIR)
#  else
#    error "No implementation of S_ISDIR available."
#  endif
#endif

// }}}

namespace {

//
// bw_stat()    {{{
//

#if defined(HAVE_STAT)
typedef struct stat stat_t;

int bw_stat(const char *path, stat_t &buf)
{
    return stat(path, &buf);
}
#elif defined(HAVE__STAT)
typedef struct _stat stat_t;

int bw_stat(const char *path, stat_t &buf)
{
    return _stat(path, &buf);
}
#else
#  error "Neither stat() nor _stat() are available on the system."
#endif

// }}}

//
// bw_mkdir()   {{{
//

#if defined(HAVE__MKDIR)
int bw_mkdir(const char *path, uint64_t mode)
{
    (void)mode;
    return _mkdir(path);
}
#elif defined(HAVE_MKDIR)
int bw_mkdir(const char *path, uint64_t mode)
{
    return mkdir(path, mode);
}
#else
#  error "Neither mkdir() nor _mkdir() are available on the system."
#endif

// }}}

} // end anonymous namespace

namespace bw {

/* FileUtils {{{ */

int64_t FileUtils::size(const std::string &filename)
{
    int64_t size;

    std::FILE *fp = fopen(filename.c_str(), "r");
    if (!fp)
        throw SystemError("Unable to determine the size of '" + filename + "'", errno);

    /*
     * Use C-style file I/O because the 64-bit/large file support behaviour is well-defined
     * by POSIX
     */

#ifdef HAVE_FSEEKO
    int ret = fseeko(fp, 0, SEEK_END);
#else
    int ret = fseek(fp, 0, SEEK_END);
#endif

    if (ret != 0) {
        fclose(fp);
        throw SystemError("Unable to seek to the end of '" + filename + "'", errno);
    }

#ifdef HAVE_FTELLO
    size = ftello(fp);
#else
    size = ftell(fp);
#endif

    fclose(fp);

    return size;
}

bool FileUtils::exists(const std::string &filename)
{
    stat_t statresult;
    return bw_stat(filename.c_str(), statresult) == 0;
}

bool FileUtils::isDirectory(const std::string &dirname)
{
    stat_t statresult;
    int err = bw_stat(dirname.c_str(), statresult);
    if (err < 0)
        throw SystemError("Unable to retrieve statistics for '" + dirname + "'", errno);

    return S_ISDIR(statresult.st_mode);
}

void FileUtils::mkdir(const std::string &dir, bool recursive)
{
    if (!recursive) {
        if (!FileUtils::exists(dir)) {
            int ret = bw_mkdir(dir.c_str(), 0777);
            if (ret != 0)
                throw SystemError("mkdir of " + dir + " failed.", errno);
        }
    } else {
        std::string directory = dir;

        // remove trailing '/' if there are any
        while (directory[directory.size()-1] == '/')
            directory = directory.substr(0, directory.size()-1);

        std::string::size_type current_slash = 0;

        while (true) {
            current_slash = directory.find('/', current_slash+1);
            if (current_slash == std::string::npos) {
                FileUtils::mkdir(directory, false);
                break;
            }

            mkdir(directory.substr(0, current_slash), false);
        }
    }
}

std::string FileUtils::join(const std::string &a, const std::string &b)
{
    return a + "/" + b;
}

std::string FileUtils::join(const std::string &a,
                            const std::string &b,
                            const std::string &c)
{
    return a + "/" + b + "/" + c;
}

std::string basename(const std::string &path)
{
    std::string::size_type slashPos = path.rfind('/');
    if (slashPos == std::string::npos)
        return path;
    else
        return path.substr(slashPos + 1);
}

#if defined(HAVE_GETPWUID_R)
std::string FileUtils::homeDirectory()
{
    long size = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (size < 0)
        throw SystemError("Unable to call sysconf(_SC_GETPW_R_SIZE_MAX)", errno);

    char *buffer = new char[size];
    struct passwd mypasswd;
    struct passwd *result = NULL;

    uid_t myuid = getuid();
    int ret = getpwuid_r(myuid, &mypasswd, buffer, size, &result);
    if (ret < 0)
        throw SystemError("Unable to call getpwuid_r()", errno);
    else if (!result)
        throw SystemError("Unable to call getpwuid_r(): UID " + str(myuid) + " not found.", ENOENT);

    std::string homedir(result->pw_dir);

    delete[] buffer;

    return homedir;
}
#elif defined(_WIN32)
std::string FileUtils::homeDirectory()
{
    TCHAR path[MAX_PATH];

    if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE|CSIDL_FLAG_CREATE, NULL, 0, path)))
        throw SystemError("Unable to call SHGetFolderPath");

    return std::string(path);
}
#endif

/* }}} */

} // end namespace bw
