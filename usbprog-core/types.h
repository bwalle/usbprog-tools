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
#ifndef USBPROG_CORE_TYPES_H
#define USBPROG_CORE_TYPES_H

#include <vector>
#include <map>
#include <string>

/* Typedefs {{{ */

class Device;
typedef std::vector<unsigned char> ByteVector;
typedef std::map<std::string, std::string> StringStringMap;
typedef std::vector<Device *> DeviceVector;
typedef std::vector<std::string> StringVector;

/* }}} */

#endif /* USBPROG_CORE_TYPES_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
