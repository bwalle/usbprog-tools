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

#include <libusb.h>

#include <usbpp/configdescriptor.h>
#include <usbpp/interfacedescriptor.h>
#include <usbpp/util.h>

namespace USB {

/* ConfigDescriptorPrivate {{{ */

struct ConfigDescriptorPrivate {
    libusb_config_descriptor *config_descriptor;
};

/* }}} */
/* ConfigDescriptor {{{ */

/* -------------------------------------------------------------------------- */
ConfigDescriptor::ConfigDescriptor(void *nativeHandle)
    : m_data(new ConfigDescriptorPrivate)
{
    m_data->config_descriptor = static_cast<libusb_config_descriptor *>(nativeHandle);
}

/* -------------------------------------------------------------------------- */
ConfigDescriptor::~ConfigDescriptor()
{
    libusb_free_config_descriptor(m_data->config_descriptor);
    delete m_data;
}

/* -------------------------------------------------------------------------- */
unsigned short ConfigDescriptor::getConfigurationValue() const
{
    return m_data->config_descriptor->bConfigurationValue;
}

/* -------------------------------------------------------------------------- */
size_t ConfigDescriptor::getNumberOfInterfaces() const
{
    return m_data->config_descriptor->bNumInterfaces;
}

/* -------------------------------------------------------------------------- */
size_t ConfigDescriptor::getNumberOfAltsettings(int interfaceNumber) const
    throw (Error)
{
    if (interfaceNumber < 0 || interfaceNumber >= getNumberOfInterfaces()) {
        std::stringstream ss;
        ss << "Interface number " << interfaceNumber << " does not exist.";
        throw Error(ss.str());
    }

    return m_data->config_descriptor->interface[interfaceNumber].num_altsetting;
}

/* -------------------------------------------------------------------------- */
InterfaceDescriptor *ConfigDescriptor::getInterfaceDescriptor(int interfaceNumber, int altsetting)
    throw (Error)
{
    if (altsetting < 0 || altsetting >= getNumberOfAltsettings(interfaceNumber)) {
        std::stringstream ss;
        ss << "Altsetting number " << altsetting << " does not exist.";
        throw Error(ss.str());
    }

    return new InterfaceDescriptor(&m_data->config_descriptor->interface[interfaceNumber].altsetting[altsetting]);
}


/* }}} */

} // end namespace USB


// vim: set sw=4 ts=4 et: :collapseFolds=1:
