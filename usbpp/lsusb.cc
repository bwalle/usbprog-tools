/*
 * (c) 2010, Bernhard Walle <bernhard@bwalle.de>
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
#include <cstdlib>
#include <iostream>

#include <usbpp/usbmanager.h>
#include <usbpp/device.h>


int main(int argc, char *argv[])
{
    usb::UsbManager &manager = usb::UsbManager::instance();
    manager.setDebug(true);
    manager.detectDevices();

    std::cout << "Number of USB devices: " << manager.getNumberOfDevices() << std::endl;
    for (size_t deviceNumber = 0; deviceNumber < manager.getNumberOfDevices(); ++deviceNumber) {
        usb::Device *dev = manager.getDevice(deviceNumber);
        std::cout << "------------------------------------------------" << std::endl;
        std::cout << dev->getDescriptor() << std::endl;
    }

    return EXIT_SUCCESS;
}


// vim: set sw=4 ts=4 et: :collapseFolds=1:
