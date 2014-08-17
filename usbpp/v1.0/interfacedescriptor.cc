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
#include <sstream>

#include "libusb_1.0.h"
#include "error.h"

#include <usbpp/interfacedescriptor.h>
#include <usbpp/configdescriptor.h>

namespace usb {

/* InterfaceDescriptorPrivate {{{ */

struct InterfaceDescriptorPrivate {
    const libusb_interface_descriptor *interface_descriptor;
};

/* }}} */
/* InterfaceDescriptor {{{ */

InterfaceDescriptor::InterfaceDescriptor(const void *nativeHandle)
    : m_data(new InterfaceDescriptorPrivate)
{
    m_data->interface_descriptor = static_cast<const libusb_interface_descriptor *>(nativeHandle);
}

InterfaceDescriptor::~InterfaceDescriptor()
{
    delete m_data;
}

unsigned short InterfaceDescriptor::getInterfaceNumber() const
{
    return m_data->interface_descriptor->bInterfaceNumber;
}

/* }}} */

} // end namespace usb


// vim: set sw=4 ts=4 et: :collapseFolds=1:
