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
 * @file devicehandle.h
 * @brief Contains the DeviceHandle
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#ifndef USBPP_DEVICE_HANDLE_H
#define USBPP_DEVICE_HANDLE_H

#include <usbpp/exceptions.h>

namespace usb {

/* Forward declarations {{{ */

struct DeviceHandlePrivate;

/* }}} */

/* Device {{{ */
/**
 * @class DeviceHandle usbpp/usbpp.h
 * @brief Handle to an opened USB device
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class DeviceHandle
{
    friend class Device;

    public:
        virtual ~DeviceHandle();

        /**
         * @brief Returns the configuration
         *
         * @return the current configuration number
         * @exception Error on any error
         */
        int getConfiguration() const;

        /**
         * @brief Sets the configuration
         *
         * @param[in] newConfiguration the number of the new configuration
         * @exception Error on any error
         */
        void setConfiguration(int newConfiguration);

        /**
         * @brief Claims an interface
         *
         * @param[in] interfaceNumber the interface to claim
         * @exception Error on any error
         */
        void claimInterface(int interfaceNumber);

        /**
         * @brief Releases an interface
         *
         * The class also keeps track of claimed interfaces and releases them in the destructor. So
         * the class is exception-safe.
         *
         * @param[in] interfaceNumber the interface number to release
         * @exception Error on any error
         */
        void releaseInterface(int interfaceNumber);

        /**
         * @brief Sets the alternate setting
         *
         * @param[in] interfaceNumber the number of the interface
         * @param[in] alternateSetting the alternate setting number
         * @exception Error on any error
         */
        void setInterfaceAltSetting(int interfaceNumber, int alternateSetting);

        /**
         * @brief Starts a control transfer
         *
         * @param[in] bmRequestType the request type
         * @param[in] bRequest the request
         * @param[in] wValue the value
         * @param[in] wIndex the index
         * @param[in] data the data that should be transferred (with length @p wLength)
         * @param[in] wLength the length of the data
         * @param[in] timeout the timeout
         * @exception Error on any error
         */
        void controlTransfer(unsigned char      bmRequestType,
                             unsigned char      bRequest,
                             unsigned short     wValue,
                             unsigned short     wIndex,
                             unsigned char      *data,
                             unsigned short     wLength,
                             unsigned int       timeout);

        /**
         * @brief Starts a bulk transfer
         *
         * @param[in] endpoint the endpoint number
         * @param[in] data the data of length @p length
         * @param[in] length the length of @p data
         * @param[out] transferred the number of transferred bytes (can be NULL)
         * @param[in] timeout the timeout
         * @exception Error on any error
         */
        void bulkTransfer(unsigned char     endpoint,
                          unsigned char     *data,
                          int               length,
                          int               *transferred,
                          unsigned int      timeout);

        /**
         * @brief Resets the device
         *
         * After resetting, this DeviceHandle is invalid. The only valid operation is to delete
         * the object and retrieve a new one.
         *
         * @exception Error on any error
         */
        void resetDevice();

    protected:
        /**
         * @brief Constructor
         *
         * Creates a new DeviceHandle object.
         *
         * @param[in] nativeHandle the libusb handle for the InterfaceDescriptor
         */
        DeviceHandle(void *nativeHandle);

    private:
        // noncopyable
        DeviceHandle(const DeviceHandle &other);
        DeviceHandle &operator=(const DeviceHandle &other);

    private:
        DeviceHandlePrivate *const m_data;
};

/* }}} */

} // end namespace usb

#endif /* USBPP_DEVICE_HANDLE_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
