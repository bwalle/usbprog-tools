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
 * @file interfacedescriptor.h
 * @brief Contains the USB InterfaceDescriptor declaration
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#ifndef USBPP_INTERFACEDESCRIPTOR_H
#define USBPP_INTERFACEDESCRIPTOR_H

#include <usbpp/exceptions.h>

namespace usb {

/* Forward declarations {{{ */

struct InterfaceDescriptorPrivate;

/* }}} */

/* InterfaceDescriptor {{{ */

/**
 * @class InterfaceDescriptor usbpp/usbpp.h
 * @brief USB interface descriptor
 *
 * C++ abstraction of the USB interface descriptor.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class InterfaceDescriptor
{
    friend class ConfigDescriptor;

public:
    /**
     * @brief Destructor
     */
    virtual ~InterfaceDescriptor();

public:
    /**
     * @brief Returns the USB interface number
     *
     * @return the interface number
     */
    unsigned short getInterfaceNumber() const;

protected:
    /**
     * @brief Constructor
     *
     * @param[in] nativeHandle the libusb handle for the InterfaceDescriptor
     */
    InterfaceDescriptor(const void *nativeHandle);

private:
    // noncopyable
    InterfaceDescriptor(const InterfaceDescriptor &other);
    InterfaceDescriptor &operator=(const InterfaceDescriptor &other);

private:
    InterfaceDescriptorPrivate *const m_data;
};

/* }}} */

} // end namespace usb

#endif /* USBPP_INTERFACEDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
