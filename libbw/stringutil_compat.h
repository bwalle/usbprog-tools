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
#ifndef STRINGUTIL_COMPAT_H
#define STRINGUTIL_COMPAT_H

/**
 * @file stringutil_compat.h
 * @brief Compatibility functions for operating systems that lack some functions
 *
 * This file currently contains compatibility functions for Win32.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#include <string.h>

#include "bwconfig.h"

namespace bw {

#if !HAVE_STRCASECMP

/**
 * @brief Compares two strings case-insensitive
 *
 * The strcasecmp() function compares the two strings s1 and s2, ignoring
 * the case of the characters. It returns an integer less than, equal to,
 * or greater than zero if s1 is found, respectively, to be less than, to
 * match, or be greater than s2.
 *
 * @param[in] s1 the first string to compare
 * @param[in] s2 the second string to compare
 * @return see above
 */
int strcasecmp(const char *s1, const char *s2)
{
     return _stricmp(s1, s2);
}

#endif /* HAVE_STRCASECMP */

} // end namespace bw

#endif /* STRINGUTIL_H */
