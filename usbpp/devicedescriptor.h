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
#ifndef USBPP_DEVICEDESCRIPTOR_H
#define USBPP_DEVICEDESCRIPTOR_H

#include <iostream>
#include <usbpp/exceptions.h>

namespace usb {

/* DeviceDescriptor {{{ */

class DeviceDescriptor
{
    public:
        unsigned short getDeviceClass() const;
        void setDeviceClass(unsigned short deviceClass);

        unsigned short getDeviceSubclass() const;
        void setDeviceSubclass(unsigned short deviceSubclass);

        unsigned int getVendorId() const;
        void setVendorId(unsigned int vendorId);

        unsigned int getProductId() const;
        void setProductId(unsigned int productId);

        unsigned short getBcdDevice() const;
        void setBcdDevice(unsigned short bcdDevice);

    private:
        unsigned short  m_bDeviceClass;
        unsigned short  m_bDeviceSubClass;
        unsigned int    m_idVendor;
        unsigned int    m_idProduct;
        unsigned short  m_bcdDevice;
};

std::ostream &operator<<(std::ostream &os, const DeviceDescriptor &desc);

/* }}} */

} // end namespace usb

#endif /* USBPP_DEVICEDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
