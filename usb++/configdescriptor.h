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
#ifndef USBPP_CONFIGDESCRIPTOR_H
#define USBPP_CONFIGDESCRIPTOR_H

#include <usb++/exceptions.h>
#include <usb++/devicedescriptor.h>

namespace USB {

/* Forward declarations {{{ */

struct ConfigDescriptorPrivate;
class InterfaceDescriptor;

/* }}} */

/* ConfigDescriptor {{{ */

class ConfigDescriptor
{
    friend class Device;

    public:
        virtual ~ConfigDescriptor();

        unsigned short getConfigurationValue() const;

        size_t getNumberOfInterfaces() const;
        size_t getNumberOfAltsettings(int interfaceNumber) const
        throw (Error);

        InterfaceDescriptor *getInterfaceDescriptor(int interfaceNumber,
                                                    int altsetting)
        throw (Error);

    protected:
        ConfigDescriptor(void *nativeHandle);

    private:
        // noncopyable
        ConfigDescriptor(const ConfigDescriptor &other);
        ConfigDescriptor &operator=(const ConfigDescriptor &other);

    private:
        ConfigDescriptorPrivate *const m_data;
};

/* }}} */

} // end namespace USB

#endif /* USBPP_CONFIGDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
