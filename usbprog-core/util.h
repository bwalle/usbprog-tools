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
#ifndef USBPROG_UTIL_H
#define USBPROG_UTIL_H

#include <usbprog-core/date.h>
#include <usbprog-core/types.h>

namespace usbprog {
namespace core {

/* Fileutil {{{ */

class Fileutil {
    public:
        static std::string homeDir();
        static std::string configDir(const std::string &program);
        static bool mkdir(const std::string &dir);
        static bool isDir(const std::string &dir);
        static bool isFile(const std::string &dir);
        static bool isPathName(const std::string &dir);
        static std::string resolvePath(const std::string &path);
        static DateTime getMTime(const std::string &file)
            throw (IOError);
        static ByteVector readBytesFromFile(const std::string &file)
            throw (IOError);
};

/* }}} */
/* Free functions {{{ */

std::string pathconcat(const std::string &a, const std::string &b);
void usbprog_sleep(unsigned int seconds);
void usbprog_msleep(unsigned int msec);

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_UTIL_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
