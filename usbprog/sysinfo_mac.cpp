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
#include <QSysInfo>

#include <usbprog/sysinfo.h>

namespace usbprog {

/* Sysinfo {{{ */

/* -------------------------------------------------------------------------- */
std::string Sysinfo::osName()
{
    return "Mac OS";
}

/* -------------------------------------------------------------------------- */
std::string Sysinfo::osVersion()
{
    switch (QSysInfo::MacintoshVersion) {
        case QSysInfo::MV_9:
            return "9";
        case QSysInfo::MV_10_0:
            return "10.0";
        case QSysInfo::MV_10_1:
            return "10.1";
        case QSysInfo::MV_10_2:
            return "10.2";
        case QSysInfo::MV_10_3:
            return "10.3";
        case QSysInfo::MV_10_4:
            return "10.4";
        case QSysInfo::MV_10_5:
            return "10.5";
        case QSysInfo::MV_10_6:
            return "10.6";
        default:
            return "unknown";
    }
}

/* }}} */

} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
