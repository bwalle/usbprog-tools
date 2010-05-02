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

set(libusbprog_MOCS
    downloader.h
)

set(libusbprog_SRCS
    firmwarepool.cc
    downloader.cc
)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_mac.cpp)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_win.cpp)
else ()
    set(libusbprog_SRCS ${libusbprog_SRCS} sysinfo_posix.cpp)
endif ()

qt4_wrap_cpp(libusbprog_MOC_SRCS ${libusbprog_MOCS})
add_library(libusbprog STATIC ${libusbprog_SRCS} ${libusbprog_MOC_SRCS})
target_link_libraries(libusbprog libusbprog-core ${EXTRA_LIBS})

# vim: set sw=4 ts=4 et:
