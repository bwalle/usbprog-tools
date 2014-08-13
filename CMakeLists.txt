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
project(usbprog CXX C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Configuration
#

cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0028 NEW)
set (PACKAGE_STRING "usbprog")
set (PACKAGE_VERSION "0.3.0")
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR})

option(BUILD_GUI "Build the Qt GUI" ON)
option(USE_QT5 "Use Qt5 instead of Qt4" OFF)
option(BUILD_ONLY_CORE "Builds only the usbprog-core lib and a simple CLI program (has no library dependencies apart from libusb" OFF)
option(USE_LEGACY_LIBUSB "Ignore the fact that libusb-1.0 is present and look
for legacy libusb 0.1" OFF)

#
# Check if the configuration is sane
#

if (BUILD_GUI AND BUILD_ONLY_CORE)
    message(SEND_ERROR "It doesn't make sense to specify 'BUILD_GUI=ON' and 'BUILD_ONLY_CORE=ON'")
endif (BUILD_GUI AND BUILD_ONLY_CORE)

#
# Check for functions
#
include (CheckFunctionExists)
check_function_exists(strptime HAVE_STRPTIME)
if (HAVE_STRPTIME)
    set (CONFIG_HAVE_STRPTIME 1)
else (HAVE_STRPTIME)
    set (CONFIG_HAVE_STRPTIME 0)
endif (HAVE_STRPTIME)

#
# Find binaries
#

find_program(POD2MAN_EXECUTABLE pod2man
    DOC "Pod2Man binary"
)

if (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")
    message("'pod2man' not found. Not building manpage.")
    set(BUILD_MANPAGE FALSE)
else (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")
    set(BUILD_MANPAGE TRUE)
endif (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")


#
# Find libraries
#

# libusb 1.0 or libusb legacy

include (Findlibusb)
if (NOT LIBUSB_FOUND)
    message(FATAL_ERROR "libusb not found.")
endif (NOT LIBUSB_FOUND)


include_directories(${LIBUSB_INCLUDE_DIRS})
set (EXTRA_LIBS ${EXTRA_LIBS} ${LIBUSB_LIBRARIES})

if (NOT BUILD_ONLY_CORE)

    if (USE_QT5)

        if (BUILD_GUI)
            find_package(Qt5Widgets)
            set(EXTRA_LIBS ${EXTRA_LIBS} Qt5::Widgets)
        else (BUILD_GUI)
            find_package(Qt5Core)
        endif (BUILD_GUI)

    else (USE_QT5)

        # Qt4
        include(FindQt4)
        if (BUILD_GUI)
            set(Qt_Components QtCore QtGui QtXml QtNetwork QtMain)
        else (BUILD_GUI)
            set(Qt_Components QtCore QtXml QtNetwork QtMain)
        endif (BUILD_GUI)

        find_package(Qt4 4.4.3 COMPONENTS ${Qt_Components} REQUIRED)
        include(${QT_USE_FILE})
        set(EXTRA_LIBS ${EXTRA_LIBS} ${QT_LIBRARIES})
        if (NOT QT4_FOUND)
            message(FATAL_ERROR "Qt4 not found. You can set the USE_QT5 option to try to find Qt5")
        endif (NOT QT4_FOUND)

    endif (USE_QT5)

    # libbw

    add_subdirectory(libbw)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libbw)
    set (EXTRA_LIBS ${EXTRA_LIBS} bw)

    set (EXTRA_LIBS ${EXTRA_LIBS} ${CURSES_LIBRARIES})
    include_directories(${CURSES_INCLUDE_DIR})

endif (NOT BUILD_ONLY_CORE)

if (MSVC)
    # MSVC++ ignores all exception specifications except '()' [i.e. no exception]
    # we can safely ignore that warning when compiling on MS Windows
    add_definitions("/w44290")

    # we also don't want useless warnings that we should use some Windows specific functions
    # instead of Standard C functions (which is quite right because the standard C functions
    # are not safe, but we have no alternative in portable code)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1)

    # also get rid of warnings "Function call with parameters that may be unsafe"
    # the code is correct C++!
    add_definitions(-D_SCL_SECURE_NO_WARNINGS=1)
endif (MSVC)

#
# Generate config.h
#

set (DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/usbprog")

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

#
# Include the subdirs
#
add_subdirectory(md5)
add_subdirectory(usbprog-core)
add_subdirectory(usbpp)
add_subdirectory(cli-basic)
if (NOT BUILD_ONLY_CORE)
    add_subdirectory(usbprog)
    add_subdirectory(cli)
    add_subdirectory(gui)
endif (NOT BUILD_ONLY_CORE)
add_subdirectory(udev)

#
# Print status
#

message(STATUS "Build only the core parts   : ${BUILD_GUI}")
message(STATUS "Building with GUI           : ${BUILD_GUI}")
message(STATUS "Building manpages           : ${BUILD_MANPAGE}")

# vim: set sw=4 ts=4 et:
