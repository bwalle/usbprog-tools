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
#ifndef USBPP_DEVICE_HANDLE_H
#define USBPP_DEVICE_HANDLE_H

#include <usbpp/exceptions.h>

namespace USB {

/* Forward declarations {{{ */

struct DeviceHandlePrivate;

/* }}} */

/* Device {{{ */

class DeviceHandle
{
    friend class Device;

    public:
        virtual ~DeviceHandle();

        int getConfiguration() const
        throw (Error);

        void setConfiguration(int newConfiguration)
        throw (Error);

        void claimInterface(int interfaceNumber)
        throw (Error);

        void releaseInterface(int interfaceNumber)
        throw (Error);

        void setInterfaceAltSetting(int interfaceNumber, int alternateSetting)
        throw (Error);

        void controlTransfer(unsigned char      bmRequestType,
                             unsigned char      bRequest,
                             unsigned short     wValue,
                             unsigned short     wIndex,
                             unsigned char      *data,
                             unsigned short     wLength,
                             unsigned int       timeout)
        throw (Error);

        void bulkTransfer(unsigned char     endpoint,
                          unsigned char     *data,
                          int               length,
                          int               *transferred,
                          unsigned int      timeout)
        throw (Error);

    protected:
        DeviceHandle(void *nativeHandle);

    private:
        // noncopyable
        DeviceHandle(const DeviceHandle &other);
        DeviceHandle &operator=(const DeviceHandle &other);

    private:
        DeviceHandlePrivate *const m_data;
};

/* }}} */

} // end namespace USB

#endif /* USBPP_DEVICE_HANDLE_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
