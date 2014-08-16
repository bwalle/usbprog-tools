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
 * @file stringutil.h
 * @brief Classes and free functions to work with strings
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef USBPROG_STRINGUTIL_H
#define USBPROG_STRINGUTIL_H

#include <string>

#include <usbprog-core/types.h>

namespace usbprog {
namespace core {

/* StringTokenizer interface {{{ */

/**
 * @brief Simple string tokenizer
 *
 * Interface to split a string into tokens. This is only an interface class, the only implementation
 * available is ShellStringTokenizer.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class StringTokenizer {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~StringTokenizer() {}

public:
    /**
     * @brief Checks if there are more tokens available
     *
     * @return @c true if there are more tokens, @c false otherwise
     */
    virtual bool hasMoreTokens() const = 0;

    /**
     * @brief Returns the next token
     *
     * If hasMoreTokens() returns @c true, then this function returns the next token
     * as string.
     *
     * @return the token
     */
    virtual std::string nextToken() = 0;

    /**
     * @brief Returns the result of a tokenize action
     *
     * This function can be used instead of calling hasMoreTokens() and nextToken() in
     * a loop to tokenize a string.
     *
     * @return the tokens
     */
    virtual StringVector tokenize() = 0;
};

/* }}} */
/* ShellStringTokenizer {{{ */

/**
 * @brief Tokenizer with shell semantics
 *
 * Strings are separated with spaces. Quotation marks are honored and inside a string enclosed
 * in quotation marks, the backslash character can be used to quote.
 *
 * Example: The string <tt>bla \"fasel \\"fasel\\" bla\"</tt> is split into two tokens: <tt>'bla'</tt>
 * is the first one and <tt>'fasel "fasel" bla'</tt> the second one.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class ShellStringTokenizer : public StringTokenizer {
public:
    /**
     * @brief Creates a new ShellStringTokenizer object
     *
     * @param[in] string the string to tokenize
     */
    ShellStringTokenizer(const std::string &string);

public:
    virtual bool hasMoreTokens() const;
    virtual std::string nextToken();

    virtual StringVector tokenize();

private:
    std::string m_string;
    size_t m_pos;
};

/* }}} */
/* global functions {{{ */

/**
 * @brief Strips whitespace characters from @p a
 *
 * @param[in] a the string to strip which is not modified itself
 * @return the stripped string
 * @ingroup core
 */
std::string strip(std::string a);

/**
 * @brief Extracts a numeric value from @p string
 *
 * When @p string starts with <tt>'0x</tt>, then this prefix is stripped and the value is interpreted
 * as hexadecimal value. Otherwise, the value is interpreted decimal.
 *
 * @param[in] string the C-string to parse
 * @return the long value
 * @ingroup core
 */
unsigned long parse_long(const char *string);

/**
 * @brief Wraps @p text so that the result has @p margin columns
 *
 * @param[in] text the text to wrap
 * @param[in] margins the number of columns which the resulting text should have
 * @return the formatted string
 * @ingroup core
 */
std::string wordwrap(const std::string &text, int margins);

/**
 * @brief Converts a string vector to a C-string array
 *
 * The result must be freed by the caller.
 *
 * @param[in] vec the string vector
 * @return the two-dimensional array
 * @ingroup core
 */
char **stringvector_to_array(const StringVector &vec);

/**
 * @brief Checks if @p string starts with @p start
 *
 * @param[in] string the reference string which should be checked
 * @param[in] start the string which should be used to check if @p string starts with it
 * @return @c true if @p string starts with @p start, @c false otherwise
 * @ingroup core
 */
bool str_starts_with(const std::string &string, const std::string &start);

/**
 * @brief Returns a string vector that contains one empty string
 *
 * This is just a short alias for
 *
 * @code
 * std::vector<std::string> vec;
 * vec.push_back("");
 * @endcode
 *
 * @return the string vector as described above
 * @ingroup core
 */
std::vector<std::string> empty_element_sv();

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_STRINGUTIL_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
