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
#include <QApplication>
#include <QTime>

#include <usbprog-core/debug.h>

#include "qtsleeper.h"

namespace usbprog {
namespace gui {

/* QtSleeper {{{ */

void QtSleeper::sleep(int ms)
{
    USBPROG_DEBUG_DBG("Qtsleeper: sleep %d ms", ms);
    QTime t;
    t.start();

    while (t.elapsed() < ms)
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents, ms - t.elapsed());
}

/* }}} */

} // end namespace gui
} // end namespace usbprog


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
