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
#include <stdexcept>
#include <iostream>

#include <QApplication>
#include <QMessageBox>
#include <QNetworkInterface>

#include "usbprog_app.h"

int main(int argc, char *argv[])
{
    usbprog::gui::UsbprogApplication app(argc, argv);

    try {
        int rc;
        if (!app.parseCommandLine(argc, argv, rc))
            return rc;
        app.createAndSetMainWidget();

    } catch (const std::runtime_error &err) {
        QMessageBox::warning(NULL, usbprog::gui::UsbprogApplication::NAME,
                             QObject::tr("An unknown exception has occurred:\n%1").arg(err.what()));
        return EXIT_FAILURE;
    }

    return qApp->exec();
}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
