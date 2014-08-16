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
 * @file usbmanager.h
 * @brief Entry point for usbpp
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#ifndef USBPP_LIBUSB_H
#define USBPP_LIBUSB_H

#include <usbpp/exceptions.h>

namespace usb {

struct UsbManagerPrivate;
class Device;

/* UsbManager {{{ */

/**
 * @class UsbManager usbpp/usbpp.h
 * @brief Entry point for usbpp (singleton)
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class UsbManager
{
public:
    /**
     * @brief Singleton accessor
     *
     * Returns a reference to the only instance of UsbManager
     *
     * @return the reference as described above
     * @exception Error if creating the UsbManager instance failed. That exception an be only thrown
     *            in the first invocation of that method.
     */
    static UsbManager &instance();

public:
    /**
     * @brief Enables or disables USB debugging
     *
     * @param[in] debug @c true if debugging should be enabled, @c false otherwise
     */
    void setDebug(bool debug);

    /**
     * @brief Detects the devices
     *
     * This function must be called every time new devices are attached or old devices are removed,
     * i.e. to keep the information up to date.
     */
    void detectDevices();

    /**
     * @brief Returns the number of currently attached devices
     *
     * @return the number of devices
     */
    size_t getNumberOfDevices() const;

    /**
     * @brief Returns the device number @p number
     *
     * @param[in] number the device number that must be between 0 (inclusive) and getNumberOfDevices()
     *            (exclusive)
     * @return a pointer to the Device object. The pointer is still owned by the DeviceManager and must not
     *         be freed by the caller.
     * @exception std::out_of_range if @p number is out of range.
     */
    Device *getDevice(size_t number);

private:
    // make c'tor and d'tor private
    UsbManager();
    virtual ~UsbManager();

    // disable copy c-tor and assignment operator
    UsbManager(const UsbManager &other);
    UsbManager &operator=(const UsbManager &other);

private:
    UsbManagerPrivate *const m_data;
};

/* }}} */

} // end namespace usb

#endif /* USBPP_LIBUSB_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
