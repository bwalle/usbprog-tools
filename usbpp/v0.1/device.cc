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
#include "libusb_0.1.h"

#include <usbpp/device.h>
#include <usbpp/devicehandle.h>
#include <usbpp/configdescriptor.h>

namespace usb {

/* DevicePrivate {{{ */

struct DevicePrivate {
    struct usb_device *device;
};

/* }}} */
/* Device {{{ */

Device::~Device()
{
    delete m_data;
}

unsigned short Device::getDeviceNumber() const
{
    return m_data->device->devnum;
}

unsigned short Device::getBusNumber() const
{
    return m_data->device->bus->location;
}

Device::Device(void *nativeHandle)
    : m_data(new DevicePrivate)
{
    m_data->device = static_cast<struct usb_device *>(nativeHandle);
}

DeviceDescriptor Device::getDescriptor() const
{
    DeviceDescriptor ret;

    struct usb_device_descriptor usbDescriptor = m_data->device->descriptor;

    ret.setDeviceClass(usbDescriptor.bDeviceClass);
    ret.setDeviceSubclass(usbDescriptor.bDeviceSubClass);
    ret.setVendorId(usbDescriptor.idVendor);
    ret.setProductId(usbDescriptor.idProduct);
    ret.setBcdDevice(usbDescriptor.bcdDevice);

    return ret;
}

ConfigDescriptor *Device::getConfigDescriptor(int index)
{
    struct usb_config_descriptor *usb_config_descriptor = m_data->device->config;

    return new ConfigDescriptor(usb_config_descriptor);
}

DeviceHandle *Device::open()
{
    usb_dev_handle *handle = usb_open(m_data->device);
    if (!handle)
        throw Error(usb_strerror());

    return new DeviceHandle(handle);
}


/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
