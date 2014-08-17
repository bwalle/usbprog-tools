/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <string>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <fstream>

#include <usbprog-core/util.h>
#include <usbprog-core/date.h>

#ifdef _WIN32
#  include <windows.h>
#  include <shlobj.h>
#else
#  include <pwd.h>
#  include <sys/types.h>
#  include <unistd.h>
#  include <pwd.h>
#endif

#include <sys/stat.h>
#include "oscompat.h"

/* Defines {{{ */

#define BUFFERSIZE       2048

/* }}} */

namespace usbprog {
namespace core {

/* Fileutil {{{ */

#ifdef _WIN32
std::string Fileutil::homeDir()
{
    TCHAR path[MAX_PATH];

    if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE|CSIDL_FLAG_CREATE,
                    NULL, 0, path)))
        return std::string(path);
    else
        return std::string();
}
#else
std::string Fileutil::homeDir()
{
    struct passwd *pw;

    pw = getpwuid(getuid());
    if (pw)
        return std::string(pw->pw_dir);
    else
        return std::string();
}
#endif

#ifdef _WIN32
std::string Fileutil::configDir(const std::string &program)
{
    TCHAR path[MAX_PATH];

    if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA|CSIDL_FLAG_CREATE,
                    NULL, 0, path)))
        return std::string(path) + "/" + program;
    else
        return std::string();
}
#else
std::string Fileutil::configDir(const std::string &program)
{
    return homeDir() + "/." + program;
}
#endif

#ifdef _WIN32
bool Fileutil::mkdir(const std::string &dir)
{
    return SUCCEEDED(CreateDirectory(dir.c_str(), NULL));
}
#else
bool Fileutil::mkdir(const std::string &dir)
{
    return ::mkdir(dir.c_str(), 0777) == 0;
}
#endif

bool Fileutil::isDir(const std::string &dir)
{
    int         ret;
    struct stat my_stat;

    ret = stat(dir.c_str(), &my_stat);
    if (ret < 0)
        return false;

    return S_ISDIR(my_stat.st_mode);
}

bool Fileutil::isFile(const std::string &file)
{
    int         ret;
    struct stat my_stat;

    ret = stat(file.c_str(), &my_stat);
    if (ret < 0)
        return false;

    return S_ISREG(my_stat.st_mode);
}

DateTime Fileutil::getMTime(const std::string &file)
{
    int         ret;
    struct stat my_stat;

    ret = stat(file.c_str(), &my_stat);
    if (ret < 0)
        throw IOError("File " + file + " does not exist.");

    return DateTime(my_stat.st_mtime);
}

#if _WIN32
bool Fileutil::isPathName(const std::string &file)
{
    return file.find("/") != std::string::npos || isFile(file);
}
#else
bool Fileutil::isPathName(const std::string &file)
{
    return file.find("/") != std::string::npos ||
        isFile(file) || file[0] == '~';
}
#endif

#ifdef _WIN32
std::string Fileutil::resolvePath(const std::string &path)
{
    return path;
}
#else
std::string Fileutil::resolvePath(const std::string &path)
{
    if (path[0] != '~')
        return path;
    else if (path[1] == '/') {
        const char *home = getpwuid(getuid())->pw_dir;
        if (!home || strlen(home) <= 0)
            return path;

        return pathconcat(home, path.substr(1));
    } else {
        size_t end_user = path.find('/');
        if (end_user == std::string::npos)
            return path;

        std::string username = path.substr(1, end_user-1);
        struct passwd *pw = getpwnam(username.c_str());
        if (!pw)
            return path;

        return pathconcat(pw->pw_dir, path.substr(end_user));
    }
}
#endif

ByteVector Fileutil::readBytesFromFile(const std::string &file)
{
    ByteVector bv;
    char buffer[BUFFERSIZE];

    std::ifstream fin(file.c_str(), std::ios::binary);
    if (!fin)
        throw core::IOError("Opening " + file + " failed");

    while (!fin.eof()) {
        fin.read(buffer, BUFFERSIZE);
        if (fin.bad())
            throw core::IOError("Error while reading data from " + file);

        std::copy(buffer, buffer + fin.gcount(), back_inserter(bv));
    }

    fin.close();
    return bv;
}

/* }}} */
/* global {{{ */

#ifdef _WIN32
std::string pathconcat(const std::string &a, const std::string &b)
{
    return a + "\\" + b;
}
#else
std::string pathconcat(const std::string &a, const std::string &b)
{
    return a + "/" + b;
}
#endif

void usbprog_sleep(unsigned int seconds)
{
    usbprog_msleep(seconds * 1000);
}

#ifdef _WIN32
void usbprog_msleep(unsigned int msec)
{
    Sleep(msec);
}
#else
void usbprog_msleep(unsigned int msec)
{
    struct timespec ts;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000 * 1000;
    nanosleep(&ts, NULL);
}
#endif

/* }}} */

} // end namespace core
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
