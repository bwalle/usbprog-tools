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
#ifndef USBPP_LIBUSB_H
#define USBPP_LIBUSB_H

#include <usb++/exceptions.h>

namespace USB {

struct UsbManagerPrivate;
class Device;

class UsbManager
{
    public:
        static UsbManager &instance() throw (Error);

    public:
        void setDebuglevel(int debuglevel);

        void detectDevices();
        size_t getNumberOfDevices() const;
        Device *getDevice(size_t number)
        throw (std::out_of_range);

    private:
        // make c'tor and d'tor private
        UsbManager() throw (Error);
        virtual ~UsbManager();

        // disable copy c-tor and assignment operator
        UsbManager(const UsbManager &other);
        UsbManager &operator=(const UsbManager &other);

    private:
        UsbManagerPrivate *const m_data;
};

} // end namespace USB

#endif /* USBPP_LIBUSB_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
