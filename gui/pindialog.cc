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
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>

#include "pindialog.h"

namespace usbprog {
namespace gui {

/* PinDialog {{{ */

PinDialog::PinDialog(Firmware *fw, QWidget *parent)
    : QDialog(parent)
    , m_firmware(fw)
{
    setupUI();
    layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void PinDialog::setupUI()
{
    QString labels[] = {
        "P1",       "P2",
        "P3",       "P4",
        "P5",       "P6",
        "P7",       "P8",
        "P9",       "P10",
        "LED",      "JP4",
        "TX",       "RX"
    };

    QVBoxLayout *mainLayout = new QVBoxLayout();

    // the info box on the left
    QWidget *pinGrid = new QWidget(this);
    QGridLayout *pinGridLayout = new QGridLayout;

    pinGridLayout->setColumnMinimumWidth(2, 30);
    for (int line = 0; line < (sizeof(labels)/sizeof(labels[0])/2); line ++) {
        for (int column = 0; column < 2; column++) {
            int index = line*2 + column;
            QLabel *key = new QLabel( QString("<b>%1</b>").arg(labels[index]) );
            std::string pinName(labels[index].toStdString());
            QLabel *value = new QLabel( QString::fromStdString(m_firmware->getPin(pinName)) );

            int tableColumn(column == 0 ? 0 : 3);
            pinGridLayout->addWidget(key, line, tableColumn);
            pinGridLayout->addWidget(value, line, tableColumn+1);
        }
    }
    pinGridLayout->setRowStretch(sizeof(labels)/sizeof(labels[0])/2, 10);

    pinGrid->setLayout(pinGridLayout);

    // the pixmap on the right
    QLabel *rightLabel = new QLabel(this);
    rightLabel->setPixmap(QPixmap(":/pins.xpm"));
    rightLabel->setAlignment(Qt::AlignTop);

    // top box
    QWidget *topBox = new QWidget(this);
    QHBoxLayout *topBoxLayout = new QHBoxLayout;
    topBox->setLayout(topBoxLayout);
    topBoxLayout->addWidget(pinGrid);
    topBoxLayout->addWidget(rightLabel);
    topBoxLayout->setSpacing(25);
    mainLayout->addWidget(topBox);

    // buttons on the bottom
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, Qt::Horizontal, this);
    mainLayout->addWidget(buttonBox);
    mainLayout->setSpacing(20);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    setLayout(mainLayout);
    setWindowTitle(tr("Pin assignment for %1").arg( QString::fromStdString(m_firmware->getName())) );
}

/* }}} */

} // end namespace gui
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
