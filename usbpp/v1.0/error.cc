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
#include "error.h"

namespace usb {

const char *errorcodeToString(int err)
{
    static const char *errortable[] = {
        /*  0 */ "Success",
        /*  1 */ "Input/output error",
        /*  2 */ "Invalid parameter",
        /*  3 */ "Access denied (insufficient permissions)",
        /*  4 */ "No such device (it may have been disconnected)",
        /*  5 */ "Entity not found",
        /*  6 */ "Resource busy",
        /*  7 */ "Operation timed out",
        /*  8 */ "Overflow",
        /*  9 */ "Pipe error",
        /* 10 */ "System call interrupted (perhaps due to signal)",
        /* 11 */ "Insufficient memory",
        /* 12 */ "Operation not supported or unimplemented on this platform",
    };
    static const char *other = "Other error";

    const int MAX_ERRORNUMBER = sizeof(errortable)/sizeof(errortable[0]);
    if (-err >= MAX_ERRORNUMBER || -err < 0)
        return other;
    else
        return errortable[-err];
}

} // end namespace usb
