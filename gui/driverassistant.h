/*
 * (c) 2014, Bernhard Walle <bernhard@bwalle.de>
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
 * @file driverassistant.h
 * @brief Assitant to install the USBprog driver
 *
 * This file contains the class ZadigRunner.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef GUI_DRIVERASSISTANT_H_
#define GUI_DRIVERASSISTANT_H_

#include <QWizard>

namespace usbprog {
namespace gui {

class DriverAssistant : public QWizard
{
    Q_OBJECT

public:
    DriverAssistant(QWidget *parent=0);

private:
    QWizardPage *createIntroPage();
};

} // namespace gui
} // namespace usbprog

#endif // GUI_DRIVERASSISTANT_H_
