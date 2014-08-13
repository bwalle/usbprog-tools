/* {{{
 * Copyright (c) 2011, Bernhard Walle <bernhard@bwalle.de>
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

#ifndef LIBBW_COMPILER_H_
#define LIBBW_COMPILER_H_

#include "bwerror.h"
#include <vector>

/**
 * \brief Returns the size of a statically defined array
 *
 * \param[in] array the array
 * \ingroup misc
 */
#define BW_ARRAY_SIZE(array) \
    (sizeof(array)/sizeof((array)[0]))

/**
 * \brief Enables syntax checking for printf()
 *
 * Used for printf() wrappers like logging functions. Currently available for
 * GCC only.
 *
 * \param[in] string_index specifies which argument is the format string
 *            argument (starting from 1) should be checked
 * \param[in] first_to_check the number of the first argument
 *            to check against the format string. For functions where the
 *            arguments are not available to be checked (such as \c vprintf),
 *            specify as zero.
 * \ingroup misc
 */
#ifdef __GNUC__
#define BW_COMPILER_PRINTF_FORMAT(string_index, first_to_check) \
    __attribute__(( format(printf, string_index, first_to_check) ))
#else
#define BW_COMPILER_PRINTF_FORMAT(string_index, first_to_check)
#endif

/**
 * \brief Enables syntax checking for strftime()
 *
 * Used for strftime() wrappers. Currently available for GCC only.
 *
 * \param[in] string_index specifies which argument is the format string
 *            argument (starting from 1) should be checked
 * \param[in] first_to_check the number of the first argument
 *            to check against the format string. For functions where the
 *            arguments are not available to be checked (such as \c vprintf),
 *            specify as zero.
 * \ingroup misc
 */
#ifdef __GNUC__
#define BW_COMPILER_STRFTIME_FORMAT(string_index, first_to_check) \
    __attribute__(( format(strftime, string_index, first_to_check) ))
#else
#define BW_COMPILER_STRFTIME_FORMAT(string_index, first_to_check)
#endif

#endif /* LIBBW_COMPILER_H_ */
