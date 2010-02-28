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
# Find libraries
#

# libcurl
INCLUDE(FindCURL)
IF(NOT CURL_FOUND)
    MESSAGE(FATAL_ERROR "CURL not found.")
ENDIF(NOT CURL_FOUND)

SET(EXTRA_LIBS ${EXTRA_LIBS} ${CURL_LIBRARIES})
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})

# wxWidgets
INCLUDE(FindwxWidgets)
FIND_PACKAGE(wxWidgets COMPONENTS base core)
IF (wxWidgets_FOUND)
    SET(BUILD_GUI TRUE)
    INCLUDE(${wxWidgets_USE_FILE})
    SET(EXTRA_LIBS ${EXTRA_LIBS} ${wxWidgets_LIBRARIES})
ELSE (wxWidgets_FOUND)
    SET(BUILD_GUI FALSE)
ENDIF (wxWidgets_FOUND)


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

# libxml2
INCLUDE(FindLibXml2)
IF (NOT LIBXML2_FOUND)
    MESSAGE(FATAL_ERROR "libxml2 not found.")
ENDIF (NOT LIBXML2_FOUND)

INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
SET (EXTRA_LIBS ${EXTRA_LIBS} ${LIBXML2_LIBRARIES})

# libusb
INCLUDE (Findlibusb)
IF (NOT LIBUSB_FOUND)
    MESSAGE(FATAL_ERROR "libusb not found.")
ENDIF (NOT LIBUSB_FOUND)

INCLUDE_DIRECTORIES(${LIBUSB_INCLUDE_DIRS})
SET (EXTRA_LIBS ${EXTRA_LIBS} ${LIBUSB_LIBRARIES})

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
ADD_SUBDIRECTORY(usbprog)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(gui)

#
# Print status
#

MESSAGE(STATUS "Building with GUI     : ${BUILD_GUI}")
MESSAGE(STATUS "Building with readline: ${BUILD_READLINE}")

# vim: set sw=4 ts=4 et:
