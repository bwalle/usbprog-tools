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
#ifndef USBPP_EXCEPTIONS_H
#define USBPP_EXCEPTIONS_H

/**
 * @file exceptions.h
 * @brief Exception classes used in usbpp.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */

#include <stdexcept>
#include <string>

namespace usb {

/* Error {{{ */

/**
 * @class Error usbpp/usbpp.h
 * @brief USB error
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbpp
 */
class Error : public std::runtime_error {
public:
    /**
     * @brief Constructor
     *
     * Creates a new instance of Error.
     *
     * @param[in] string the error string
     */
    Error(const std::string& string)
        : std::runtime_error(string) {}
};

/* }}} */

} // end namespace usb


#endif // USBPP_EXCEPTIONS_H
