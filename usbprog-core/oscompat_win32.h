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
#ifndef OSCOMPAT_WIN32_H
#define OSCOMPAT_WIN32_H

#if !defined(S_ISREG) || !defined(S_ISDIR)

#  define __S_ISTYPE(mode, mask)    (((mode) & S_IFMT) == (mask))

#  if !defined(S_ISREG)
#    define S_ISREG(mode)  __S_ISTYPE((mode), S_IFREG)
#  endif

#  if !defined(S_ISDIR)
#    define S_ISDIR(mode)  __S_ISTYPE((mode), S_IFDIR)
#  endif

#endif


#endif // OSCOMPAT_WIN32_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
