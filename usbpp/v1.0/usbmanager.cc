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

#include "libusb_1.0.h"
#include "error.h"

#include <usbpp/usbmanager.h>
#include <usbpp/device.h>

namespace usb {

/* UsbManagerPrivate {{{ */

struct UsbManagerPrivate {
    libusb_context          *context;
    libusb_device           **devicelist;
    size_t                  device_number;
    std::vector<Device *>   devices;
};

/* }}} */
/* UsbManager {{{ */

UsbManager::UsbManager()
  : m_data(new UsbManagerPrivate)
{
    int err = libusb_init(&m_data->context);
    if (err != 0)
        throw Error(errorcodeToString(err));

    m_data->devicelist = NULL;
    m_data->device_number = 0;
}

UsbManager::~UsbManager()
{
    for (size_t i = 0; i < m_data->device_number; ++i)
        delete m_data->devices[i];
    m_data->devices.clear();
    libusb_free_device_list(m_data->devicelist, true);
    libusb_exit(m_data->context);
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
        libusb_set_debug(m_data->context, 3);
    else
        libusb_set_debug(m_data->context, 0);
}

void UsbManager::detectDevices()
{
    if (m_data->devicelist != NULL) {
        for (size_t i = 0; i < m_data->device_number; ++i)
            delete m_data->devices[i];
        m_data->devices.clear();
        libusb_free_device_list(m_data->devicelist, true);
    }

    m_data->device_number = libusb_get_device_list(m_data->context,
                                                   &m_data->devicelist);

    for (size_t i = 0; i < m_data->device_number; ++i)
        m_data->devices.push_back(new Device(m_data->devicelist[i]));
}

size_t UsbManager::getNumberOfDevices() const
{
    return m_data->device_number;
}

Device *UsbManager::getDevice(size_t number)
{
    if (number >= m_data->device_number) {
        std::stringstream ss;
        ss << "Device number " << number << " out of range";
        throw std::out_of_range(ss.str());
    }

    return m_data->devices[number];
}

/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
