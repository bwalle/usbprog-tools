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
 * @file error.h
 * @brief Exception classes
 *
 * This file contains the exception classes used in the usbprog-core lib.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef USBPROG_CORE_ERROR_H
#define USBPROG_CORE_ERROR_H

#include <stdexcept>

namespace usbprog {
namespace core {

/* IOError {{{ */

/**
 * @brief I/O Error
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class IOError : public std::runtime_error {
public:
    /**
     * @brief Constructor
     *
     * Creates a new I/O error.
     *
     * @param[in] string the error string
     */
    IOError(const std::string& string)
        : std::runtime_error(string) {}
};

/* }}} */
/* ParseError {{{ */

/**
 * @brief Parse Error
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class ParseError : public std::runtime_error {
public:
    /**
     * @brief Constructor
     *
     * Creates a new ParseError instance.
     *
     * @param[in] string the error string
     */
    ParseError(const std::string& string)
        : std::runtime_error(string) {}
};

/* }}} */
/* ApplicationError {{{ */

/**
 * @brief Application Error
 *
 * Applications errors are errors that cannot be further specified.
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class ApplicationError : public std::runtime_error {
public:
    /**
     * @brief Constructor
     *
     * Creates a new ApplicationError instance.
     *
     * @param[in] string the error string
     */
    ApplicationError(const std::string& string)
        : std::runtime_error(string) {}
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_CORE_ERROR_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
