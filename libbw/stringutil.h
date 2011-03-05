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
#ifndef STRINGUTIL_H
#define STRINGUTIL_H

/**
 * @file stringutil.h
 * @brief String helper functions
 *
 * This file contains global functions for working with strings.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#include <string>
#include <vector>

namespace bw {

/**
 * @brief Removes spaces from a string
 *
 * Removes trailing and leading characters. All characters in @p chars_to_strip
 * are removed. By default this are spaces, tabs and newlines.
 *
 * @param[in] a the string to strip (this string is not modified)
 * @param[in] chars_to_strip the characters that should be removed, defaults
 *            to whitespace
 * @return the stripped string
 */
std::string strip(std::string a, const std::string &chars_to_strip="\t \n");

/**
 * @brief Removes spaces from a string on the right side
 *
 * Removes leading spaces, tabs and newlines.
 *
 * @param[in] a the string to strip (this string is not modified)
 * @return the stripped string
 */
std::string stripr(std::string a);

/**
 * @brief Removes spaces from a string on the left side
 *
 * Removes trailing spaces, tabs and newlines.
 *
 * @param[in] a the string to strip  (this string is not modified)
 * @return the stripped string
 */
std::string stripl(std::string a);

/**
 * @brief Checks if a string starts with another string
 *
 * Checks if @p str starts with @p start. If @p casesensitive is @c true, the
 * matching is done case-sensitive.
 *
 * @param[in] str the string which is checked if it starts with @p start
 * @param[in] start the string which is taken to check if @p str starts with
 *            @p start
 * @param[in] casesensitive if @c true, the comparison is done case-sensitive
 * @return @c true if @p str starts with @p start, @c false otherwise
 */
bool startsWith(const std::string &str, const std::string &start, bool casesensitive = true);

/**
 * @brief Returns the rest
 *
 * If @p str starts with @p prefix, returns the rest, i.e @p str without @p
 * prefix. If @p str does not start with @p prefix, @p str is returned
 * unmodified.
 *
 * @param[in] str the base string
 * @param[in] prefix the prefix which is stripped
 * @return the rest string as described above
 */
std::string getRest(const std::string &str, const std::string &prefix);

/**
 * @brief Converts a string vector to a C array
 *
 * Converts a std::vector<std::string> to a char ** array. If the size of the
 * vector @p vec is 0, @c NULL is returned.
 *
 * @param[in] vec the vector to convert
 * @return the C array. The memory needs to be freed with free() after using
 *         the return value.
 */
char **stringvector_to_array(const std::vector<std::string> &vec);

/**
 * @brief Splits a string
 *
 * Splits a string in a vector. For example, if @p str is
 * <tt>"abra:kadabra"</tt> and if @p pattern is <tt>":"</tt> (both without
 * the quotes), then the resulting vector contains two elements, being
 * <tt>"abra"</tt> the first and <tt>"kadabra"</tt> the second.
 *
 * @param[in] str the string to split
 * @param[in] pattern the pattern which is used as separator
 * @return the result of the string split. If @p str does not contain @p
 *         pattern, the result is just @p str.
 */
std::vector<std::string> stringsplit(const std::string &str, const std::string &pattern);

/**
 * @brief Replaces a character with a character or a string
 *
 * This original string is not modified.
 *
 * @param[in] input the input string
 * @param[in] old_char the old character
 * @param[in] new_string the new string
 * @return the replaced string
 */
std::string replace_char(const std::string  &input,
                         char               old_char,
                         const std::string  &new_string);


#endif /* STRINGUTIL_H */

} // end namespace bw

// :tabSize=4:indentSize=4:noTabs=true:mode=c++:folding=explicit:collapseFolds=1:maxLineLen=100:
