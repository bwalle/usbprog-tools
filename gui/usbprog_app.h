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
#ifndef USBPROG_APP_H
#define USBPROG_APP_H

#include <QApplication>

#include <usbprog-core/error.h>

#include "usbprog_mainwindow.h"

namespace usbprog {
namespace gui {

/* UsbprogApplication {{{ */

class UsbprogApplication : public QApplication
{
    public:
        static const QString NAME;
        static const QString USBPROG_WEBSITE;

    public:
        UsbprogApplication(int &argc, char **argv)
        throw (core::ApplicationError);

    public:
        bool parseCommandLine(int argc, char *argv[], int &rc);
        void createAndSetMainWidget();

    protected:
        void initConfig()
        throw (core::ApplicationError);

    private:
        UsbprogMainWindow *m_mainWindow;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // USBPROG_APP_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
