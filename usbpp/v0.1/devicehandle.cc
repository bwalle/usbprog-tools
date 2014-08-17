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

#include "libusb_0.1.h"

#include <usbpp/devicehandle.h>

namespace usb {

/* DeviceHandlePrivate {{{ */

struct DeviceHandlePrivate {
    usb_dev_handle       *device_handle;
    std::list<int>       claimed_interfaces;
};

/* }}} */
/* DeviceHandle {{{ */

DeviceHandle::~DeviceHandle()
{
    for (std::list<int>::iterator it = m_data->claimed_interfaces.begin();
         it != m_data->claimed_interfaces.end(); ++it) {
        usb_release_interface(m_data->device_handle, *it);
    }
    usb_close(m_data->device_handle);
    delete m_data;
}

DeviceHandle::DeviceHandle(void *nativeHandle)
    : m_data(new DeviceHandlePrivate)
{
    m_data->device_handle = static_cast<usb_dev_handle *>(nativeHandle);
}

int DeviceHandle::getConfiguration() const
{
    // XXX: I'm not sure if that's right
    return usb_device(m_data->device_handle)->config[0].bConfigurationValue;
}

void DeviceHandle::setConfiguration(int newConfiguration)
{
    int err = usb_set_configuration(m_data->device_handle, newConfiguration);
    if (err != 0)
        throw Error(usb_strerror());
}

void DeviceHandle::claimInterface(int interfaceNumber)
{
    int err = usb_claim_interface(m_data->device_handle, interfaceNumber);
    if (err != 0)
        throw Error(usb_strerror());

    m_data->claimed_interfaces.push_back(interfaceNumber);
}

void DeviceHandle::releaseInterface(int interfaceNumber)
{
    int err = usb_release_interface(m_data->device_handle, interfaceNumber);
    if (err != 0)
        throw Error(usb_strerror());

    std::list<int>::iterator result = std::find(m_data->claimed_interfaces.begin(),
                                                m_data->claimed_interfaces.end(),
                                                interfaceNumber);
    if (result != m_data->claimed_interfaces.end())
        m_data->claimed_interfaces.erase(result);
}

void DeviceHandle::setInterfaceAltSetting(int interfaceNumber, int alternateSetting)
{
    int err = usb_set_altinterface(m_data->device_handle, alternateSetting);
    if (err != 0)
        throw Error(usb_strerror());
}

void DeviceHandle::controlTransfer(unsigned char      bmRequestType,
                                   unsigned char      bRequest,
                                   unsigned short     wValue,
                                   unsigned short     wIndex,
                                   unsigned char      *data,
                                   unsigned short     wLength,
                                   unsigned int       timeout)
{
    int err = usb_control_msg(m_data->device_handle, bmRequestType, bRequest, wValue, wIndex,
                              reinterpret_cast<char *>(data), wLength, timeout);
    if (err != 0)
        throw Error(usb_strerror());
}

void DeviceHandle::bulkTransfer(unsigned char     endpoint,
                                unsigned char     *data,
                                int               length,
                                int               *transferred,
                                unsigned int      timeout)
{
    int ret = usb_bulk_write(m_data->device_handle, endpoint, reinterpret_cast<char *>(data), length, timeout);
    if (ret < 0)
        throw Error(usb_strerror());

    if (transferred)
        *transferred = ret;
}

void DeviceHandle::resetDevice()
{
    int err = usb_reset(m_data->device_handle);
    if (err != 0)
        throw Error(usb_strerror());
}

/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
