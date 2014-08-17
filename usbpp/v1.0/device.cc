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
#include "libusb_1.0.h"
#include "error.h"

#include <usbpp/device.h>
#include <usbpp/devicehandle.h>
#include <usbpp/configdescriptor.h>

namespace usb {

/* DevicePrivate {{{ */

struct DevicePrivate {
    libusb_device *device;
};

/* }}} */
/* Device {{{ */

Device::~Device()
{
    delete m_data;
}

unsigned short Device::getDeviceNumber() const
{
    return libusb_get_device_address(m_data->device);
}

unsigned short Device::getBusNumber() const
{
    return libusb_get_bus_number(m_data->device);
}

Device::Device(void *nativeHandle)
    : m_data(new DevicePrivate)
{
    m_data->device = static_cast<libusb_device *>(nativeHandle);
}

DeviceDescriptor Device::getDescriptor() const
{
    DeviceDescriptor ret;

    struct libusb_device_descriptor usbDescriptor;
    int err = libusb_get_device_descriptor(m_data->device, &usbDescriptor);
    if (err != 0)
        throw Error(errorcodeToString(err));

    ret.setDeviceClass(usbDescriptor.bDeviceClass);
    ret.setDeviceSubclass(usbDescriptor.bDeviceSubClass);
    ret.setVendorId(usbDescriptor.idVendor);
    ret.setProductId(usbDescriptor.idProduct);
    ret.setBcdDevice(usbDescriptor.bcdDevice);

    return ret;
}

ConfigDescriptor *Device::getConfigDescriptor(int index)
{
    struct libusb_config_descriptor *usb_config_descriptor;
    int err = libusb_get_config_descriptor(m_data->device, index, &usb_config_descriptor);
    if (err != 0)
        throw Error(errorcodeToString(err));

    return new ConfigDescriptor(usb_config_descriptor);
}

DeviceHandle *Device::open()
{
    libusb_device_handle *handle;
    int err = libusb_open(m_data->device, &handle);
    if (err != 0)
        throw Error(errorcodeToString(err));

    return new DeviceHandle(handle);
}


/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
