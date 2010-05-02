/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include <QCoreApplication>

#include <usbprog/usbprog.h>

#include "usbprog.h"
#include "io.h"

int main(int argc, char *argv[])
{
    usbprog::cli::Usbprog usbprog(argc, argv);

    try {
        usbprog.initConfig();
        usbprog.parseCommandLine();
        usbprog.initFirmwarePool();
        usbprog.initDeviceManager();
        usbprog.exec();
    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// vim: set sw=4 ts=4 et: :collapseFolds=1:
