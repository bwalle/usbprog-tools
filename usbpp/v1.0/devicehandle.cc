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
#include <list>
#include <algorithm>

#include "libusb_1.0.h"
#include "error.h"

#include <usbpp/devicehandle.h>

namespace usb {

/* DeviceHandlePrivate {{{ */

struct DeviceHandlePrivate {
    libusb_device_handle *device_handle;
    std::list<int>       claimed_interfaces;
};

/* }}} */
/* DeviceHandle {{{ */

DeviceHandle::~DeviceHandle()
{
    for (std::list<int>::iterator it = m_data->claimed_interfaces.begin();
         it != m_data->claimed_interfaces.end(); ++it) {
        libusb_release_interface(m_data->device_handle, *it);
    }
    libusb_close(m_data->device_handle);
    delete m_data;
}

DeviceHandle::DeviceHandle(void *nativeHandle)
    : m_data(new DeviceHandlePrivate)
{
    m_data->device_handle = static_cast<libusb_device_handle *>(nativeHandle);
}

int DeviceHandle::getConfiguration() const
{
    int configuration;
    int err = libusb_get_configuration(m_data->device_handle, &configuration);
    if (err != 0)
        throw Error(errorcodeToString(err));

    return configuration;
}

void DeviceHandle::setConfiguration(int newConfiguration)
{
    int err = libusb_set_configuration(m_data->device_handle, newConfiguration);
    if (err != 0)
        throw Error(errorcodeToString(err));
}

void DeviceHandle::claimInterface(int interfaceNumber)
{
    int err = libusb_claim_interface(m_data->device_handle, interfaceNumber);
    if (err != 0)
        throw Error(errorcodeToString(err));

    m_data->claimed_interfaces.push_back(interfaceNumber);
}

void DeviceHandle::releaseInterface(int interfaceNumber)
{
    int err = libusb_release_interface(m_data->device_handle, interfaceNumber);
    if (err != 0)
        throw Error(errorcodeToString(err));

    std::list<int>::iterator result = std::find(m_data->claimed_interfaces.begin(),
                                                m_data->claimed_interfaces.end(),
                                                interfaceNumber);
    if (result != m_data->claimed_interfaces.end())
        m_data->claimed_interfaces.erase(result);
}

void DeviceHandle::setInterfaceAltSetting(int interfaceNumber, int alternateSetting)
{
    int err =libusb_set_interface_alt_setting(m_data->device_handle, interfaceNumber, alternateSetting);
    if (err != 0)
        throw Error(errorcodeToString(err));
}

void DeviceHandle::controlTransfer(unsigned char      bmRequestType,
                                   unsigned char      bRequest,
                                   unsigned short     wValue,
                                   unsigned short     wIndex,
                                   unsigned char      *data,
                                   unsigned short     wLength,
                                   unsigned int       timeout)
{
    int err = libusb_control_transfer(m_data->device_handle, bmRequestType, bRequest,
                                      wValue, wIndex, data, wLength, timeout);
    if (err != 0)
        throw Error(errorcodeToString(err));
}

void DeviceHandle::bulkTransfer(unsigned char     endpoint,
                                unsigned char     *data,
                                int               length,
                                int               *transferred,
                                unsigned int      timeout)
{
    int dummy;
    if (transferred == NULL)
        transferred = &dummy;

    int err = libusb_bulk_transfer(m_data->device_handle, endpoint, data, length,
                                   transferred, timeout);
    if (err != 0)
        throw Error(errorcodeToString(err));
}

void DeviceHandle::resetDevice()
{
    int err = libusb_reset_device(m_data->device_handle);
    if (err != 0)
        throw Error(errorcodeToString(err));
}

/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
