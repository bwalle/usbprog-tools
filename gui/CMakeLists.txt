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

if (BUILD_GUI)

    set(usbprog_gui_SRCS
        main.cc
        usbprog_app.cc
        usbprog_mainwindow.cc
        guiconfiguration.cc
        qtsleeper.cc
        pindialog.cc
    )
    if (WIN32)
        set(usbprog_gui_SRCS ${usbprog_gui_SRCS} usbprog_gui.rc)
    endif (WIN32)
    set(usbprog_gui_MOCS
        usbprog_mainwindow.h
        pindialog.h
    )

    # Compiled-in pixmaps
    set(QtApp_RCCS usbprog.qrc)

    # generate rules for building source files from the resources
    if (USE_QT5)
        qt5_add_resources(usbprog_gui_RCC_SRCS usbprog.qrc)
    else (USE_QT5)
        qt4_add_resources(usbprog_gui_RCC_SRCS usbprog.qrc)
    endif (USE_QT5)

    # generate rules for building source files that moc generates
    if (USE_QT5)
        qt5_wrap_cpp(usbprog_gui_MOC_SRCS ${usbprog_gui_MOCS})
    else (USE_QT5)
        qt4_wrap_cpp(usbprog_gui_MOC_SRCS ${usbprog_gui_MOCS})
    endif (USE_QT5)

    # build sources, moc'd sources, and rcc'd sources
    add_executable(usbprog-gui WIN32
        ${usbprog_gui_SRCS} ${usbprog_gui_MOC_SRCS} ${usbprog_gui_RCC_SRCS}
    )

    target_link_libraries(usbprog-gui ${EXTRA_LIBS} libusbprog-core libusbprog md5)

    install(
        TARGETS         usbprog-gui
        DESTINATION     bin
    )

    if (BUILD_MANPAGE)
        add_custom_command(
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

        add_custom_target(
            gui-manpage
            DEPENDS
                ${CMAKE_CURRENT_BINARY_DIR}/usbprog-gui.1
        )

        add_dependencies(
            usbprog-gui
            gui-manpage
        )


        install(
            FILES           ${CMAKE_CURRENT_BINARY_DIR}/usbprog-gui.1
            DESTINATION     share/man/man1
        )

    endif (BUILD_MANPAGE)

endif (BUILD_GUI)


# vim: set sw=4 ts=4 et:
