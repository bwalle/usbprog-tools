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

/**
 * @file device.h
 * @brief USB device
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
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

/**
 * @class Device usbpp/usbpp.h
 * @brief A USB device
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class Device
{
    friend class UsbManager;

    public:
        /**
         * @brief Destructor
         */
        virtual ~Device();

        /**
         * @brief Returns the device number
         *
         * @return the device number (on the bus)
         * @see getBusNumber()
         */
        unsigned short getDeviceNumber() const;

        /**
         * @brief Returns the bus number
         *
         * @return the bus number
         * @see getDeviceNumber()
         */
        unsigned short getBusNumber() const;

        /**
         * @brief Returns the device descriptor
         *
         * @return the device descriptor of the device
         * @exception Error on any error
         */
        DeviceDescriptor getDescriptor() const;

        /**
         * @brief Returns the config descriptor for a specific configuration
         *
         * @param[in] index the configuration number (see the DeviceDescriptor for the number of configurations)
         * @return the config descriptor for configuration @p index
         * @exception Error on any error
         */
        ConfigDescriptor *getConfigDescriptor(int index);

        /**
         * @brief Opens the device and returns a DeviceHandle
         *
         * @return the DeviceHandle object that must be deleted by the caller
         * @exception Error on any error
         */
        DeviceHandle *open();

        // delete the DeviceHandle to close the device

    protected:
        /**
         * @brief Constructor
         *
         * Creates a new device.
         *
         * @param[in] nativeHandle the libusb handle for the InterfaceDescriptor
         */
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
