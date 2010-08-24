#
# (c) 2010, Bernhard Walle <bernhard@bwalle.de>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.
#

# find the udev version
pkg_check_modules(UDEV udev)

if (${UDEV_VERSION} VERSION_GREATER 147)
    set(ATTR_KEYWORD "ATTR")
else (${UDEV_VERSION} VERSION_GREATER 147)
    set(ATTR_KEYWORD "SYSFS")
endif (${UDEV_VERSION} VERSION_GREATER 147)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/usbprog.rules.in"
    "${CMAKE_CURRENT_BINARY_DIR}/usbprog.rules"
)

# vim: set sw=4 ts=4 et:
