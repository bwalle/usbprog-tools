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

SET(libusbprog_MOCS
    downloader.h
)

SET(libusbprog_SRCS
    firmwarepool.cc
    downloader.cc
)

IF (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    SET(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_mac.cpp)
ELSEIF (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    SET(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_win.cpp)
ELSE ()
    SET(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_posix.cpp)
ENDIF ()

QT4_WRAP_CPP(libusbprog_MOC_SRCS ${libusbprog_MOCS})
ADD_LIBRARY(libusbprog STATIC ${libusbprog_SRCS} ${libusbprog_MOC_SRCS})
TARGET_LINK_LIBRARIES(libusbprog libusbprog-core ${EXTRA_LIBS})

# vim: set sw=4 ts=4 et:
