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
#include <sstream>
#include <vector>
#include <cassert>

#include "libusb_0.1.h"

#include <usbpp/usbmanager.h>
#include <usbpp/device.h>

namespace usb {

/* UsbManagerPrivate {{{ */

struct UsbManagerPrivate {
    std::vector<Device *>   devices;
};

/* }}} */
/* UsbManager {{{ */

UsbManager::UsbManager()
  : m_data(new UsbManagerPrivate)
{
    usb_init();
}

UsbManager::~UsbManager()
{
    for (size_t i = 0; i < m_data->devices.size(); ++i)
        delete m_data->devices[i];
    m_data->devices.clear();
    delete m_data;
}

UsbManager &UsbManager::instance()
{
    static UsbManager instance;
    return instance;
}

void UsbManager::setDebug(bool debug)
{
    if (debug)
        usb_set_debug(255);
    else
        usb_set_debug(0);
}

void UsbManager::detectDevices()
{
    for (size_t i = 0; i < m_data->devices.size(); ++i)
        delete m_data->devices[i];
    m_data->devices.clear();

    usb_find_busses();
    usb_find_devices();

    for (struct usb_bus *bus = usb_get_busses(); bus; bus = bus->next) {
        for (struct usb_device *dev = bus->devices; dev; dev = dev->next)
            m_data->devices.push_back(new Device(dev));
    }
}

size_t UsbManager::getNumberOfDevices() const
{
    return m_data->devices.size();
}

Device *UsbManager::getDevice(size_t number)
{
    if (number >= m_data->devices.size()) {
        std::stringstream ss;
        ss << "Device number " << number << " out of range";
        throw std::out_of_range(ss.str());
    }

    return m_data->devices[number];
}

/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
