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
 * @file devicedescriptor.h
 * @brief USB device descriptor
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#ifndef USBPP_DEVICEDESCRIPTOR_H
#define USBPP_DEVICEDESCRIPTOR_H

#include <iostream>
#include <usbpp/exceptions.h>

namespace usb {

/* DeviceDescriptor {{{ */

/**
 * @class DeviceDescriptor usbpp/usbpp.h
 * @brief USB device descriptor
 *
 * C++ abstraction of the USB device descriptor.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class DeviceDescriptor
{
public:
    /**
     * @brief Returns the USB device class
     *
     * @return the device class
     */
    unsigned short getDeviceClass() const;

    /**
     * @brief Set the USB device class
     *
     * @param[in] deviceClass the USB device class
     */
    void setDeviceClass(unsigned short deviceClass);

    /**
     * @brief Returns the USB device subclass
     *
     * @return the device subclass
     */
    unsigned short getDeviceSubclass() const;

    /**
     * @brief Set the USB device subclass
     *
     * @param[in] deviceSubclass the USB device subclass */
    void setDeviceSubclass(unsigned short deviceSubclass);

    /**
     * @brief Returns the vendor ID
     *
     * @return the vendor ID
     */
    unsigned int getVendorId() const;

    /**
     * @brief Set the USB vendor ID
     *
     * @param[in] vendorId the USB vendor ID
     */
    void setVendorId(unsigned int vendorId);

    /**
     * @brief Returns the USB produt ID
     *
     * @return the product ID
     */
    unsigned int getProductId() const;

    /**
     * @brief Set the USB product ID
     *
     * @param[in] productId the product ID
     */
    void setProductId(unsigned int productId);

    /**
     * @brief Returns the USB BCD Device ID
     *
     * @return the device ID
     */
    unsigned short getBcdDevice() const;

    /**
     * @brief Sets the USB BCD device ID
     *
     * @param[in] bcdDevice the device ID
     */
    void setBcdDevice(unsigned short bcdDevice);

private:
    unsigned short  m_bDeviceClass;
    unsigned short  m_bDeviceSubClass;
    unsigned int    m_idVendor;
    unsigned int    m_idProduct;
    unsigned short  m_bcdDevice;
};

/**
 * @brief Prints a USB device descriptor
 *
 * @param[in,out] os the output stream where the DeviceDescriptor should be pritned to
 * @param[in] desc the USB device descriptor that should be printed
 * @return the output stream @p os
 * @ingroup usbpp
 */
std::ostream &operator<<(std::ostream &os, const DeviceDescriptor &desc);

/* }}} */

} // end namespace usb

#endif /* USBPP_DEVICEDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
