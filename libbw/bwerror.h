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
#ifndef LIBBW_BWERROR_H_
#define LIBBW_BWERROR_H_

/**
 * \defgroup error Error handling
 * \brief Error handling classes
 *
 * This module contains classes for error/exception handling.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>
#include <cstring>
#include <cerrno>

namespace bw {

/* Error {{{ */

/**
 * \class Error bwerror.h libbw/error.h
 * \brief Base class for all exceptions in that library.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup error
 */
class Error : public std::runtime_error {
public:
    /**
     * \brief Constructor
     *
     * Creates a new Error.
     *
     * \param[in] string the error string
     */
    Error(const std::string &string)
        : std::runtime_error(string) {}
};

/* }}} */
/* IOError {{{ */

/**
 * \class IOError bwerror.h libbw/error.h
 * \brief Input/Output error
 *
 * This exception class is thrown when an Input/Output error occurred. Use the
 * std::runtime_error::what() method to retrieve the error message in your
 * exception handler.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup error
 */
class IOError : public Error {
public:
    /**
     * \brief Constructor
     *
     * Creates a new IOError.
     *
     * \param[in] string the error string
     */
    IOError(const std::string& string)
        : Error(string) {}
};

/* }}} */
/* SystemError {{{ */

/**
 * \brief Standard error class for system errors that have a valid errno information.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup error
 */
class SystemError : public Error {
public:
    /**
     * \brief Creates a new object of SystemError with string as error message.
     *
     * \param[in] string the error message
     * \param[in] errorcode the system error code (errno)
     */
    SystemError(const std::string &string, int errorcode=errno)
        : Error(string)
        , m_errorcode(errorcode)
    {
        m_errorstring = string + " (" + std::strerror(m_errorcode) + ")";
    }

    /**
     * \brief Returns a readable error message from the string and the error code.
     *
     * \return Error message in the format 'string (strerror(errorcode))'.
     */
    virtual const char *what() const throw ()
    {
        return m_errorstring.c_str();
    }

    /**
     * Don't know why that is necessary to avoid compiler errors.
     */
    virtual ~SystemError() throw () {}

private:
    int m_errorcode;
    std::string m_errorstring;
};

/* }}} */
/* SystemIOError {{{ */

/**
 * \brief Error class for I/O system errors that have a valid errno information.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup error
 */
class SystemIOError : public IOError {
public:
    /**
     * \brief Creates a new object of SystemError with string as error message.
     *
     * \param[in] string the error message
     * \param[in] errorcode the system error code (errno)
     */
    SystemIOError(const std::string &string, int errorcode)
        : IOError(string)
        , m_errorcode(errorcode)
    {
        m_errorstring = string + " (" + std::strerror(m_errorcode) + ")";
    }

    /**
     * \brief Returns a readable error message from the string and the error code.
     *
     * \return Error message in the format 'string (strerror(errorcode))'.
     */
    virtual const char *what() const throw ()
    {
        return m_errorstring.c_str();
    }

    /**
     * Don't know why that is necessary to avoid compiler errors.
     */
    virtual ~SystemIOError() throw () {}

private:
    int m_errorcode;
    std::string m_errorstring;
};

/* }}} */

} // end namespace usb

#endif // LIBBW_BWERROR_H_
