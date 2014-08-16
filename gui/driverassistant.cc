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

#include <QLabel>
#include <QVBoxLayout>

#include "driverassistant.h"

namespace usbprog {
namespace gui {

DriverAssistant::DriverAssistant(QWidget *parent)
    : QWizard(parent)
{
    addPage(createIntroPage());
}

QWizardPage *DriverAssistant::createIntroPage()
{
    QWizardPage *page = new QWizardPage;
    page->setTitle("Introduction");

    QLabel *label = new QLabel(
        tr("This wizard will help you register your copy of Super Product Two.")
    );
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

} // namespace gui
} // namespace usbprog
