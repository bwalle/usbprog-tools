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
#include <iomanip>

#include <usbpp/devicedescriptor.h>

namespace usb {

/* DeviceDescriptor {{{ */

unsigned short DeviceDescriptor::getDeviceClass() const
{
    return m_bDeviceClass;
}

void DeviceDescriptor::setDeviceClass(unsigned short deviceClass)
{
    m_bDeviceClass = deviceClass;
}

unsigned short DeviceDescriptor::getDeviceSubclass() const
{
    return m_bDeviceSubClass;
}

void DeviceDescriptor::setDeviceSubclass(unsigned short deviceSubclass)
{
    m_bDeviceSubClass = deviceSubclass;
}

unsigned int DeviceDescriptor::getVendorId() const
{
    return m_idVendor;
}

void DeviceDescriptor::setVendorId(unsigned int vendorId)
{
    m_idVendor = vendorId;
}

unsigned int DeviceDescriptor::getProductId() const
{
    return m_idProduct;
}

void DeviceDescriptor::setProductId(unsigned int productId)
{
    m_idProduct = productId;
}

unsigned short DeviceDescriptor::getBcdDevice() const
{
    return m_bcdDevice;
}

void DeviceDescriptor::setBcdDevice(unsigned short bcdDevice)
{
    m_bcdDevice = bcdDevice;
}

/* }}} */

std::ostream &operator<<(std::ostream &os, const DeviceDescriptor &desc)
{
    os << "Class:       0x" << std::hex << std::setw(2) << std::setfill('0')
                            << desc.getDeviceClass() << std::endl;
    os << "Subclass:    0x" << std::hex << std::setw(2) << std::setfill('0')
                            << desc.getDeviceSubclass() << std::endl;
    os << "Vendor ID:   0x" << std::hex << std::setw(4) << std::setfill('0')
                            << desc.getVendorId() << std::endl;
    os << "Product ID:  0x" << std::hex << std::setw(4) << std::setfill('0')
                            << desc.getProductId() << std::endl;
    os << "BCD Device:  0x" << std::hex << std::setw(2) << std::setfill('0')
                            << desc.getBcdDevice() << std::endl;
    os << std::dec;
    return os;
}

} // end namespace usb

// vim: set sw=4 ts=4 et: :collapseFolds=1:
