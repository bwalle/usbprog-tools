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
#ifndef USBPP_DEVICE_H
#define USBPP_DEVICE_H

#include <usbpp/exceptions.h>
#include <usbpp/devicedescriptor.h>

namespace usb {

/* Forward declarations {{{ */

class DeviceHandle;
class UsbManager;
class ConfigDescriptor;
struct DevicePrivate;

/* }}} */

/* Device {{{ */

class Device
{
    friend class UsbManager;

    public:
        virtual ~Device();

        unsigned short getDeviceNumber() const;
        unsigned short getBusNumber() const;

        DeviceDescriptor getDescriptor() const
        throw (Error);

        ConfigDescriptor *getConfigDescriptor(int index)
        throw (Error);

        DeviceHandle *open()
        throw (Error);

        // delete the DeviceHandle to close the device

    protected:
        Device(void *nativeHandle);

    private:
        // noncopyable
        Device(const Device &other);
        Device &operator=(const Device &other);

    private:
        DevicePrivate *const m_data;
};

/* }}} */

} // end namespace usb

#endif /* USBPP_DEVICE_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
