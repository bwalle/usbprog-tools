/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
 * @file digest.h
 * @ingroup core
 * @brief Calculate a hash of some bytes
 *
 * This file contains classes and functions that abstract from the concrete hash algorithm and
 * provide a generic interface to calculate hashes. However, currently only MD5 is implemented.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#ifndef USBPROG_DIGEST_H
#define USBPROG_DIGEST_H

#include <string>

#include <usbprog-core/error.h>

namespace usbprog {
namespace core {

/* Digest {{{ */

/**
 * @brief Base class for all digest algorithms
 *
 * Each digest algorithm is identified by an enum value like Digest::DA_MD5. Use the static function
 * Digest::create() to generate a concrete object to calculate the digest. For example:
 *
 * @code
 * std::auto_ptr<Digest> digest(Digest::create(Digest::DA_MD5));
 * @endcode
 *
 * Then you can use the functions Digest::process() and Digest::end() to calculate the digest value.
 * Consider also using the free function check_digest() which can calculate the hash sum of a file directly
 * and compare that against a reference value.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class Digest {
public:
    /**
     * @brief Different hash algorithms like MD5
     */
    enum Algorithm {
        DA_MD5      /**< the well-known MD5 algorithm which is not secure any more but can be used
                         to check against transmission errors */
    };

public:
    /**
     * @brief Destructor
     */
    virtual ~Digest() {}

public:
    /**
     * @brief Adds the bytes of @p buffer to the digest calculation
     *
     * This member function may be called multiple times.
     *
     * @param[in] buffer the buffer that contains @p len bytes
     * @param[in] len the length of the buffer
     */
    virtual void process(unsigned char *buffer, size_t len) = 0;

    /**
     * @brief Finishes the calculation and returns the result
     *
     * Returns the result as hex string which is quite usual (the shell utilities
     * @c md5sum or @c sha1sum do the same).
     *
     * @return the digest string in textual representation
     */
    virtual std::string end() = 0;

    /**
     * @brief Factory function
     *
     * Create a concrete implementation of Digest with @p algorithm.
     *
     * @param[in] algorithm the digest algorith, e.g. Digest::DA_MD5.
     * @return a newly created instance of Digest (which must be freed by the caller) or @c NULL if
     *         @p algorithm contains an invalid value.
     */
    static Digest *create(enum Algorithm algorithm);
};

/* }}} */
/* MD5Digest {{{ */

/**
 * @brief Implementation of Digest for MD5
 *
 * Don't use that class directly. Instead, use Digest::create() to obtain an implementation.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class MD5Digest : public Digest {
public:
    MD5Digest();
    ~MD5Digest();

public:
    void process(unsigned char *buffer, size_t len);
    std::string end();

private:
    void *m_md5; /* using void here because that header file should
                    not include md5 stuff and a simple forward declaration
                    because md5_t is "typedef struct { } ..." instead of
                    "struct ... { }" */
};

/* }}} */
/* check_digest() {{{ */

/**
 * @brief Checks the hash sum of @p file against @p reference
 *
 * Reads the file @p file and calculates the hash sum using the algorithm
 * @p da and compares that against the reference value in @p reference.
 *
 * @param[in] file the path of the file (which may be relative) to read
 * @param[in] reference the reference hash sum in string format (see Digest::end())
 * @param[in] da the hash algorithm that should be used to compare against
 * @return @c true if the reference digest matches the computed one, @c false otherwise
 * @exception IOError if @p file could not be read
 * @ingroup core
 */
bool check_digest(const std::string     &file,
                  const std::string     &reference,
                  Digest::Algorithm     da);


/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_DIGEST_H */

// vim: set sw=4 ts=4 foldmethod=marker et: :collapseFolds=1:
