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

set(usbprog_SRC
    commands.cc
    io.cc
    main.cc
    shell.cc
    usbprog.cc
    cliconfiguration.cc
)

if (WIN32)
    set(usbprog_SRC ${usbprog_SRC} usbprog.rc)
endif (WIN32)

add_executable(usbprog ${usbprog_SRC})
target_link_libraries(usbprog ${EXTRA_LIBS} libusbprog md5)

install(
    TARGETS         usbprog
    DESTINATION     bin
)

if (BUILD_MANPAGE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog.1
        COMMAND
            ${POD2MAN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/usbprog.pod
            --center="USBprog" --release=${PACKAGE_VERSION} > usbprog.1
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/usbprog.pod
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_target(
        manpages
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog.1
    )

    add_dependencies(
        usbprog
        manpages
    )

    install(
        FILES           ${CMAKE_CURRENT_BINARY_DIR}/usbprog.1
        DESTINATION     share/man/man1
    )
endif (BUILD_MANPAGE)

# vim: set sw=4 ts=4 et:
