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
    return "Windows";
}

/* -------------------------------------------------------------------------- */
std::string Sysinfo::osVersion()
{
    switch (QSysInfo::WinVersion()) {
        case QSysInfo::WV_32s:
            return "3.1";
        case QSysInfo::WV_95:
            return "95";
        case QSysInfo::WV_98:
            return "98";
        case QSysInfo::WV_Me:
            return "ME";
        case QSysInfo::WV_NT:
            return "NT 4.0";
        case QSysInfo::WV_2000:
            return "2000";
        case QSysInfo::WV_XP:
            return "XP";
        case QSysInfo::WV_2003:
            return "Server 2003";
        case QSysInfo::WV_VISTA:
            return "Vista";
        case QSysInfo::WV_WINDOWS7:
            return "7";
        default:
            return "unknown";
    }
}

/* }}} */

} // end namespace usbprog


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
