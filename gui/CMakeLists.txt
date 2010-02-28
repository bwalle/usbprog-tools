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

IF (BUILD_GUI)
    ADD_EXECUTABLE(usbprog-gui
        viewer.cc
        pindialog.cc
        infodialog.cc
        usbprogApp.cc
        usbprogFrm.cc
    )
    TARGET_LINK_LIBRARIES(usbprog-gui ${EXTRA_LIBS} md5 libusbprog)
ENDIF (BUILD_GUI)

# vim: set sw=4 ts=4 et:
