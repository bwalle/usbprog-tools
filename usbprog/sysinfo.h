/*
 * (c) 2010, Bernhard Walle <bernhard@bwalle.de>
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

/**
 * @file sysinfo.h
 * @brief Platform-independent system information
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */

#ifndef SYSINFO_H
#define SYSINFO_H

#include <string>

namespace usbprog {

/* Sysinfo {{{ */

/**
 * @class Sysinfo usbprog/sysinfo.h
 * @brief System information
 *
 * This class contains only static functions to access system information.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class Sysinfo {
    public:
        /**
         * @brief Returns the name of the operating system
         *
         * Returns
         *
         *   - <tt>"Windows"</tt> on Microsoft Windows systems,
         *   - <tt>"Mac OS"</tt> on Mac OS systems,
         *   - the member <tt>struct utsname::sysname</tt> on POSIX systems except Mac OS.
         *
         * @return the name of the operating system as shown above
         */
        static std::string osName();

        /**
         * @brief Returns the version of the operating system
         *
         * Returns a version string that is suitable to identify the version of the OS.
         *
         * @return a version string
         */
        static std::string osVersion();
};

/* }}} */

} // end namespace usbprog


#endif /* SYSINFO_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
