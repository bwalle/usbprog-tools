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

add_library(libusbprog-core STATIC
        configuration.cc
        devices.cc
        stringutil.cc
        util.cc
        date.cc
        digest.cc
        inifile.cc
        debug.cc
        sleeper.cc
)
target_link_libraries(libusbprog-core ${EXTRA_LIBS} md5 usbpp)

# vim: set sw=4 ts=4 et:
