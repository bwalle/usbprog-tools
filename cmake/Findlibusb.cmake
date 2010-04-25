# - Try to find libusb
# Once done this will define
#
#  LIBUSB_FOUND - system has libusb
#  LIBUSB_INCLUDE_DIRS - the libusb include directory
#  LIBUSB_LIBRARIES - Link these to use libusb
#  LIBUSB_DEFINITIONS - Compiler switches required for using libusb
#  LIBUSB_VERSION - The string "1.0" for libusb-1.0 or "0.1" for legacy libusb.
#
# You may set following variables if that module has problems finding libusb installation.
#
#  LIBUSB_ADDITIONAL_INCLUDEDIR - additional directory where CMake should look for libusb include dir
#  LIBUSB_ADDITIONAL_LIBDIR - additional library directory where CMake should look for libusb libs
#
# Adapted from cmake-modules Google Code project
#
#  Copyright (c) 2006 Andreas Schneider <mail@cynapses.org>
#  (Changes for libusb) Copyright (c) 2008 Kyle Machulis <kyle@nonpolynomial.com>
#  (Changes for libusb 1.0) Copyright (c) 2010 Bernhard Walle <bernhard@bwalle.de>
#
# Redistribution and use is allowed according to the terms of the New BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

INCLUDE (FindPkgConfig)

# at first: try pkg-config to find libusb 1.0
PKG_CHECK_MODULES(USB10 libusb-1.0)
if (USB10_FOUND)
  set(LIBUSB_LIBRARIES ${USB10_LDFLAGS})
  if (EXISTS "${USB10_INCLUDE_DIRS}/libusb-1.0/libusb.h")
    set(LIBUSB_INCLUDE_DIRS "${USB10_INCLUDE_DIRS}/libusb-1.0")
  elseif (EXISTS "${USB10_INCLUDE_DIRS}/libusb.h")
    set(LIBUSB_INCLUDE_DIRS "${USB10_INCLUDE_DIRS}")
  endif ()
  set (LIBUSB_FOUND true)
  set (LIBUSB_VERSION 1.0)
endif (USB10_FOUND)

# then look manually for libusb 1.0
if (NOT LIBUSB_FOUND)
  find_path(LIBUSB_INCLUDE_DIR
    NAMES
      libusb.h
    PATHS
      /usr/include
      /usr/include/libusb-1.0
      /usr/local/include
      /usr/local/include/libusb-1.0
      /opt/local/include
      /opt/local/include/libusb-1.0
      /sw/include
      /sw/include/libusb-1.0
      ${LIBUSB_ADDITIONAL_INCLUDEDIR}
  )

  message("includedir=${LIBUSB_INCLUDE_DIR}")

  find_library(LIBUSB_LIBRARY
    NAMES
      libusb-1.0
      usb-1.0
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
      ${LIBUSB_ADDITIONAL_LIBDIR}
  )
  message("libdir=${LIBUSB_LIBRARY}")

  if (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIR})
    if (CMAKE_HOST_WIN32)
      set(LIBUSB_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIRS} "${LIBUSB_INCLUDE_DIR}/../msvc")
    endif (CMAKE_HOST_WIN32)

    set(LIBUSB_LIBRARIES ${LIBUSB_LIBRARY})
    set (LIBUSB_FOUND true)
    set (LIBUSB_VERSION 1.0)
  endif (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)
endif (NOT LIBUSB_FOUND)

# then look manually for libusb 0.1
if (NOT LIBUSB_FOUND)
  find_path(LIBUSB_INCLUDE_DIR
    NAMES
      usb.h
    PATHS
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      ${LIBUSB_ADDITIONAL_INCLUDEDIR}
  )

  find_library(LIBUSB_LIBRARY
    NAMES
      usb
    PATHS
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
      ${LIBUSB_ADDITIONAL_LIBDIR}
  )

  if (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)
    set(LIBUSB_INCLUDE_DIRS ${LIBUSB_INCLUDE_DIR})
    set(LIBUSB_LIBRARIES ${LIBUSB_LIBRARY})
    set (LIBUSB_FOUND true)
    set (LIBUSB_VERSION 0.1)
  endif (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARY)
endif (NOT LIBUSB_FOUND)


# show the LIBUSB_INCLUDE_DIRS and LIBUSB_LIBRARIES variables only in the advanced view
mark_as_advanced(LIBUSB_INCLUDE_DIRS LIBUSB_LIBRARIES)

