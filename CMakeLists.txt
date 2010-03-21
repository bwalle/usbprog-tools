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
PROJECT(usbprog CXX C)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Configuration
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
SET (PACKAGE_STRING "usbprog")
SET (PACKAGE_VERSION "0.3.0")
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})

OPTION(BUILD_GUI "Build the Qt GUI" ON)
OPTION(BUILD_ONLY_CORE "Builds only the usbprog-core lib and a simple CLI program (has no library dependencies apart from libusb" OFF)

#
# Check for functions
#
INCLUDE (CheckFunctionExists)
CHECK_FUNCTION_EXISTS(strptime HAVE_STRPTIME)
IF (HAVE_STRPTIME)
    SET (CONFIG_HAVE_STRPTIME 1)
ELSE (HAVE_STRPTIME)
    SET (CONFIG_HAVE_STRPTIME 0)
ENDIF (HAVE_STRPTIME)

#
# Find binaries
#

FIND_PROGRAM(POD2MAN_EXECUTABLE pod2man
    DOC "Pod2Man binary"
)

IF (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")
    MESSAGE("'pod2man' not found. Not building manpage.")
    SET(BUILD_MANPAGE FALSE)
ELSE (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")
    SET(BUILD_MANPAGE TRUE)
ENDIF (${POD2MAN_EXECUTABLE} STREQUAL "POD2MAN_EXECUTABLE-NOTFOUND")


#
# Find libraries
#

# libusb
INCLUDE (Findlibusb)
IF (NOT LIBUSB_FOUND)
    MESSAGE(FATAL_ERROR "libusb not found.")
ENDIF (NOT LIBUSB_FOUND)

INCLUDE_DIRECTORIES(${LIBUSB_INCLUDE_DIRS})
SET (EXTRA_LIBS ${EXTRA_LIBS} ${LIBUSB_LIBRARIES})

IF (NOT BUILD_ONLY_CORE)

    # Qt4
    INCLUDE(FindQt4)
    IF (BUILD_GUI)
        SET(Qt_Components QtCore QtGui QtXml QtNetwork QtMain)
    ELSE (BUILD_GUI)
        SET(Qt_Components QtCore QtXml QtNetwork QtMain)
    ENDIF (BUILD_GUI)

    FIND_PACKAGE(Qt4 4.4.3 COMPONENTS ${Qt_Components} REQUIRED)
    INCLUDE(${QT_USE_FILE})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${QT_LIBRARIES})
    IF (NOT QT4_FOUND)
        MESSAGE(FATAL_ERROR "Qt4 not found.")
    ENDIF (NOT QT4_FOUND)

    # Boost
    INCLUDE(FindBoost)
    SET(Boost_USE_STATIC_LIBS   OFF)
    SET(Boost_USE_MULTITHREADED ON)
    FIND_PACKAGE( Boost COMPONENTS program_options )
    IF (NOT Boost_FOUND)
        MESSAGE(FATAL_ERROR "Boost not found.")
    ENDIF (NOT Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

    # Readline
    FIND_PATH(READLINE_INCLUDE_DIR readline/readline.h
        /usr/local/include /usr/include)
    MARK_AS_ADVANCED(READLINE_INCLUDE_DIR)

    FIND_LIBRARY(READLINE_LIBRARY readline
        /usr/lib64 /usr/lib)
    MARK_AS_ADVANCED(READLINE_LIBRARY)

    IF (READLINE_LIBRARY AND READLINE_INCLUDE_DIR)
        INCLUDE_DIRECTORIES( ${READLINE_INCLUDE_DIR} )
        SET (EXTRA_LIBS ${EXTRA_LIBS} ${READLINE_LIBRARY})
        SET (CONFIG_HAVE_LIBREADLINE 1)
        SET (BUILD_READLINE TRUE)
    ELSE (READLINE_LIBRARY AND READLINE_INCLUDE_DIR)
        SET (CONFIG_HAVE_LIBREADLINE 0)
        SET (BUILD_READLINE FALSE)
    ENDIF (READLINE_LIBRARY AND READLINE_INCLUDE_DIR)

    IF (NOT CURSES_LIBRARIES)
        FIND_PACKAGE(Curses REQUIRED)
    ENDIF (NOT CURSES_LIBRARIES)

    SET (EXTRA_LIBS ${EXTRA_LIBS} ${CURSES_LIBRARIES})
    INCLUDE_DIRECTORIES(${CURSES_INCLUDE_DIR})

ENDIF (NOT BUILD_ONLY_CORE)

#
# Generate config.h
#

SET (DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/usbprog")

CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

#
# Include the subdirs
#
ADD_SUBDIRECTORY(md5)
ADD_SUBDIRECTORY(usbprog-core)
IF (NOT BUILD_ONLY_CORE)
    ADD_SUBDIRECTORY(usbprog)
    ADD_SUBDIRECTORY(cli)
    ADD_SUBDIRECTORY(gui)
ENDIF (NOT BUILD_ONLY_CORE)

#
# Print status
#

MESSAGE(STATUS "Build only the core parts   : ${BUILD_GUI}")
MESSAGE(STATUS "Building with GUI           : ${BUILD_GUI}")
MESSAGE(STATUS "Building with readline      : ${BUILD_READLINE}")
MESSAGE(STATUS "Building manpages           : ${BUILD_MANPAGE}")

# vim: set sw=4 ts=4 et:
