/* {{{
 * Copyright (c) 2008-2010, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */
#ifndef BWERROR_H
#define BWERROR_H

/**
 * @file bwerror.h
 * @brief Exception classes
 *
 * This file contains classes for error/exception handling.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>

namespace bw {

/* IOError {{{ */

/**
 * @class IOError bwerror.h libbw/error.h
 * @brief Input/Output error
 *
 * This exception class is thrown when an Input/Output error occurred. Use the
 * std::runtime_error::what() method to retrieve the error message in your
 * exception handler.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class IOError : public std::runtime_error {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new IOError.
         *
         * @param[in] string the error string
         */
        IOError(const std::string& string)
            : std::runtime_error(string) {}
};

/* }}} */

} // end namespace usb

#endif // BWERROR_H
