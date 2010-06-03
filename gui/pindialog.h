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
#ifndef PINDIALOG_H
#define PINDIALOG_H

#include <QDialog>

#include <usbprog-core/devices.h>
#include <usbprog/firmwarepool.h>

namespace usbprog {
namespace gui {

/* PinDialog {{{ */

/**
 * @class PinDialog gui/pindialog.h
 * @brief Pin dialog
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class PinDialog : public QDialog
{
    Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of PinDialog to display the pinning of the firmware @p fw.
         *
         * @param[in] fw the firmware for which the pin assignment should be displayed.
         * @param[in] parent the parent widget
         */
        PinDialog(Firmware *fw, QWidget *parent);

        /**
         * @brief Setup the user interface.
         */
        void setupUI();

    private:
        Firmware *m_firmware;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // PINDIALOG_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
