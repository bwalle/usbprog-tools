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
#ifndef PROGRESSNOTIFIER_H
#define PROGRESSNOTIFIER_H

namespace usbprog {
namespace core {

/* ProgressNotifier {{{ */

class ProgressNotifier {
    public:
        virtual ~ProgressNotifier() {}

    public:
        virtual int progressed(double total, double now) = 0;
        virtual void finished() = 0;
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* PROGRESSNOTIFIER_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
