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

set(usbprog_basic_SRC
    main.cc
    usbprog_basic.cc
)

if (WIN32)
    set(usbprog_basic_SRC ${usbprog_basic_SRC} usbprog_basic.rc)
endif (WIN32)

add_executable(usbprog-basic ${usbprog_basic_SRC})
target_link_libraries(usbprog-basic ${EXTRA_LIBS} libusbprog-core)

if (BUILD_ONLY_CORE)
    install(
        TARGETS         usbprog-basic
        DESTINATION     bin
    )
endif (BUILD_ONLY_CORE)

if (BUILD_MANPAGE)
    add_custom_command(
        OUTPUT
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog-basic.1
        COMMAND
            ${POD2MAN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/usbprog-basic.pod
            --center="USBprog" --release=${PACKAGE_VERSION} > usbprog-basic.1
        DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/usbprog-basic.pod
        WORKING_DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}
    )

    add_custom_target(
        basic_manpage
        DEPENDS
            ${CMAKE_CURRENT_BINARY_DIR}/usbprog-basic.1
    )

    add_dependencies(
        usbprog-basic
        basic_manpage
    )

    if (BUILD_ONLY_CORE)
        install(
            FILES           ${CMAKE_CURRENT_BINARY_DIR}/usbprog-basic.1
            DESTINATION     share/man/man1
        )
endif (BUILD_ONLY_CORE)
endif (BUILD_MANPAGE)

# vim: set sw=4 ts=4 et:
