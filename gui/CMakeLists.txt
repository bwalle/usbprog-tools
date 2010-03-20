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

    SET(usbprog_gui_SRCS
        main.cc
        usbprog_app.cc
        usbprog_mainwindow.cc
    )
    SET(usprog_gui_MOCS
        usbprog_mainwindow.h
    )

    # Compiled-in pixmaps
    SET(QtApp_RCCS usbprog.qrc)

    # generate rules for building source files from the resources
    QT4_ADD_RESOURCES(usbprog_gui_RCC_SRCS usbprog.qrc)

    # generate rules for building source files that moc generates
    QT4_WRAP_CPP(usbprog_gui_MOC_SRCS ${usbprog_gui_MOCS})

    # build sources, moc'd sources, and rcc'd sources
    ADD_EXECUTABLE(usbprog-gui WIN32
        ${usbprog_gui_SRCS} ${usbprog_gui_MOC_SRCS} ${usbprog_gui_RCC_SRCS}
    )

    TARGET_LINK_LIBRARIES(usbprog-gui ${EXTRA_LIBS} libusbprog md5)
ENDIF (BUILD_GUI)

IF (BUILD_MANPAGE AND BUILD_GUI)
    ADD_CUSTOM_COMMAND(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog-gui.1
        COMMAND
            ${POD2MAN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/usbprog-gui.pod
            --center="USBprog" --release=${PACKAGE_VERSION} > usbprog-gui.1
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/usbprog-gui.pod
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
    )

    ADD_CUSTOM_TARGET(
        gui-manpage
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog-gui.1
    )

    ADD_DEPENDENCIES(
        usbprog-gui
        gui-manpage
    )
ENDIF (BUILD_MANPAGE AND BUILD_GUI)

# vim: set sw=4 ts=4 et:
