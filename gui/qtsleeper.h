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

/**
 * @file qtsleeper.h
 * @brief Sleeper implementation that keeps the Qt event loop busy
 *
 * This file contains the QtSleeper.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef QTSLEEPER_H
#define QTSLEEPER_H

#include <usbprog-core/sleeper.h>

namespace usbprog {
namespace gui {

/* QtSleeper {{{ */

/**
 * @class QtSleeper gui/qtsleeper.h
 * @brief Sleeper that keeps the event loop busy
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class QtSleeper : public core::Sleeper
{
    public:
        /// @copydoc core::Sleeper::sleep(int)
        virtual void sleep(int ms);
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // QTSLEEPER_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
