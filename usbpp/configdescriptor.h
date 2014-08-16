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
 * @file configdescriptor.h
 * @brief USB config descriptor
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#ifndef USBPP_CONFIGDESCRIPTOR_H
#define USBPP_CONFIGDESCRIPTOR_H

#include <usbpp/exceptions.h>
#include <usbpp/devicedescriptor.h>

namespace usb {

/* Forward declarations {{{ */

struct ConfigDescriptorPrivate;
class InterfaceDescriptor;

/* }}} */

/* ConfigDescriptor {{{ */

/**
 * @brief USB configuration descriptor
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class ConfigDescriptor
{
    friend class Device;

public:
    /**
     * @brief Destructor
     */
    virtual ~ConfigDescriptor();

    /**
     * @brief Returns the configuration value
     *
     * @return the USB configuration
     */
    unsigned short getConfigurationValue() const;

    /**
     * @brief Returns the number of interfaces
     *
     * @return the number of interfaces
     */
    size_t getNumberOfInterfaces() const;

    /**
     * @brief Returns the number of alternate settings for interface @p interfaceNumber
     *
     * @param[in] interfaceNumber the interface number for which the number of alternate settings should
     *            be returned
     * @return the number of alternate settings
     * @exception Error on any error
     */
    size_t getNumberOfAltsettings(unsigned int interfaceNumber) const;

    /**
     * @brief Returns the interface descriptor for a specific interface and altsetting
     *
     * @param[in] interfaceNumber the interface number for which the interface descriptor should be returned
     * @param[in] altsetting the alternate setting for which  the interface descriptor should be returned
     * @return the InterfaceDescriptor. The pointer is still owned by the ConfigDescriptor.
     * @exception Error on any error
     */
    InterfaceDescriptor *getInterfaceDescriptor(unsigned int interfaceNumber,
                                                unsigned int altsetting);

protected:
    /**
     * @brief Constructor
     *
     * @param[in] nativeHandle the libusb handle for the InterfaceDescriptor
     */
    ConfigDescriptor(void *nativeHandle);

private:
    // noncopyable
    ConfigDescriptor(const ConfigDescriptor &other);
    ConfigDescriptor &operator=(const ConfigDescriptor &other);

private:
    ConfigDescriptorPrivate *const m_data;
};

/* }}} */

} // end namespace usb

#endif /* USBPP_CONFIGDESCRIPTOR_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
