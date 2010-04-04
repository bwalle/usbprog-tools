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
#ifndef USBPROG_USBPROG_H
#define USBPROG_USBPROG_H

#include <vector>
#include <map>
#include <stdexcept>
#include <cstdarg>

/* Preprocessor definitions {{{ */

#define DEFAULT_INDEX_URL       "http://www.ixbat.de/usbprog/versions.xml"
#define AUTO_NOT_UPDATE_TIME    10

/* }}} */
/* interface for handling messages {{{ */

enum MessageType {
    MT_STATUS
};

class OutputHandler {
    public:
        virtual ~OutputHandler() {}

    public:
        virtual void message(MessageType type, const std::string &message) = 0;
};

/* }}} */

#endif /* USBPROG_USBPROG_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
