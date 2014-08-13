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
 * @file usbprog_app.h
 * @brief Main GUI application for USBprog
 *
 * This file contains UsbprogApplication.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef USBPROG_APP_H
#define USBPROG_APP_H

#include <QApplication>

#include <usbprog-core/error.h>

#include "usbprog_mainwindow.h"

namespace usbprog {
namespace gui {

/* UsbprogApplication {{{ */

/**
 * @class UsbprogApplication gui/usbprog_app.h
 * @brief Main application for the GUI
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class UsbprogApplication : public QApplication
{
    public:
        /// Name of the application.
        static const QString NAME;

        /// Website URL for the application.
        static const QString USBPROG_WEBSITE;

    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of UsbprogApplication. The arguments of main() are just passed to
         * that constructor.
         *
         * @param[in] argc the argument count (passed to QApplication)
         * @param[in] argv the argument vector (passed to QApplication)
         * @exception core::ApplicationError if it's not possible to determine the configuration
         *            directory because of some system error
         */
        UsbprogApplication(int &argc, char **argv);

    public:
        /**
         * @brief Parses the command line
         *
         * @param[in] argc the argument cound
         * @param[in] argv the argument vector
         * @param[out] rc the return value that should be used to exit the program if that function
         *             returns @c false
         * @return @c true on success, @c false otherwise.
         */
        bool parseCommandLine(int argc, char **argv, int &rc);

        /**
         * @brief Creates and set the main widget
         */
        void createAndSetMainWidget();

    protected:
        /**
         * @brief Inits the configuration
         *
         * @exception core::ApplicationError if something failed
         */
        void initConfig();

    private:
        UsbprogMainWindow *m_mainWindow;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // USBPROG_APP_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
