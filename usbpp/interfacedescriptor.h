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
#ifndef USBPP_INTERFACEDESCRIPTOR_H
#define USBPP_INTERFACEDESCRIPTOR_H

#include <usbpp/exceptions.h>

namespace USB {

/* Forward declarations {{{ */

struct InterfaceDescriptorPrivate;

/* }}} */

/* InterfaceDescriptor {{{ */

class InterfaceDescriptor
{
    friend class ConfigDescriptor;

    public:
        virtual ~InterfaceDescriptor();

    public:
        unsigned short getInterfaceNumber() const;

    protected:
        InterfaceDescriptor(const void *nativeHandle);

    private:
        // noncopyable
        InterfaceDescriptor(const InterfaceDescriptor &other);
        InterfaceDescriptor &operator=(const InterfaceDescriptor &other);

    private:
        InterfaceDescriptorPrivate *const m_data;
};

/* }}} */

} // end namespace USB

#endif /* USBPP_INTERFACEDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
