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
#include <cstring>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>

#include "usbprog_mainwindow.h"
#include "usbprog_app.h"
#include "guiconfiguration.h"

// -----------------------------------------------------------------------------
UsbprogMainWindow::UsbprogMainWindow()
    : m_deviceManager(NULL)
    , m_firmwarepool(NULL)
{
    std::memset(&m_widgets, 0, sizeof(m_widgets));
    std::memset(&m_actions, 0, sizeof(m_actions));

    initActions();
    initWidgets();
    initMenus();
    connectSignalsAndSlots();

    // make sure that the status bar exists
    (void)statusBar();

    setWindowIcon(QPixmap(":/usbprog_icon.xpm"));
    setWindowTitle(UsbprogApplication::NAME);

    m_deviceManager = new DeviceManager;
    m_firmwarepool = new Firmwarepool(GuiConfiguration::config()->getDataDir());

    // intially populate the device list
    refreshDevices();
}

// -----------------------------------------------------------------------------
UsbprogMainWindow::~UsbprogMainWindow()
{
    delete m_deviceManager;
    delete m_firmwarepool;
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::initActions()
{
    m_actions.quit = new QAction(QIcon(":/gtk-quit.png"), tr("&Quit"), this);
    m_actions.quit->setShortcuts(QKeySequence::Quit);
    m_actions.quit->setStatusTip(tr("Quits the program"));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::connectSignalsAndSlots()
{
    connect(m_widgets.refreshButton, SIGNAL(clicked()), SLOT(refreshDevices()));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::initMenus()
{
    QMenu *programMenu = new QMenu(this);
    programMenu->setTitle(tr("&Program"));
    programMenu->addAction(m_actions.quit);

    menuBar()->addMenu(programMenu);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::initWidgets()
{
    // main widget and the layout
    m_widgets.mainWidget = new QWidget(this);
    QHBoxLayout *mainWidgetLayout = new QHBoxLayout(m_widgets.mainWidget);

    //
    // the left box with the logo and the label
    //

    m_widgets.leftBox = new QWidget(this);
    QVBoxLayout *leftBoxLayout = new QVBoxLayout(m_widgets.leftBox);

    // the logo itself
    m_widgets.usbprogGraphicsLabel = new QLabel(this);
    m_widgets.usbprogGraphicsLabel->setPixmap(QPixmap(":/usbprog.xpm"));
    m_widgets.usbprogGraphicsLabel->setAlignment(Qt::AlignCenter);

    // the link
    m_widgets.websiteLinkLabel = new QLabel(this);
    m_widgets.websiteLinkLabel->setText(tr("<a href=\"%1\">Open website</a>")
                                        .arg(UsbprogApplication::USBPROG_WEBSITE));
    m_widgets.websiteLinkLabel->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    m_widgets.websiteLinkLabel->setOpenExternalLinks(true);

    leftBoxLayout->addWidget(m_widgets.usbprogGraphicsLabel);
    leftBoxLayout->addSpacing(5);
    leftBoxLayout->addWidget(m_widgets.websiteLinkLabel);
    leftBoxLayout->addStretch(1);

    //
    // the right box with the lists, buttons and progress bar
    //

    m_widgets.rightBox = new QWidget(this);
    QVBoxLayout *rightBoxLayout = new QVBoxLayout(m_widgets.rightBox);

    m_widgets.rightTopBox = new QWidget(this);
    QHBoxLayout *rightTopBoxLayout = new QHBoxLayout(m_widgets.rightTopBox);

    m_widgets.rightMiddleBox = new QWidget(this);
    QHBoxLayout *rightMiddleBoxLayout = new QHBoxLayout(m_widgets.rightMiddleBox);

    m_widgets.rightButtonBox = new QWidget(this);
    QVBoxLayout *rightButtonBoxLayout = new QVBoxLayout(m_widgets.rightButtonBox);

    // the devices label
    m_widgets.devicesLabel = new QLabel(this);
    m_widgets.devicesLabel->setText(tr("&Devices:"));

    // devices combo box
    m_widgets.devicesCombo = new QComboBox(this);
    m_widgets.devicesLabel->setBuddy(m_widgets.devicesCombo);

    // refresh button
    m_widgets.refreshButton = new QPushButton(this);
    m_widgets.refreshButton->setText(tr("&Refresh"));
    m_widgets.refreshButton->setIcon(QPixmap(":/gtk-refresh.png"));

    // upload button
    m_widgets.uploadButton = new QPushButton(this);
    m_widgets.uploadButton->setText(tr("&Upload"));
    m_widgets.uploadButton->setIcon(QPixmap(":/gtk-go-up.png"));

    // list
    m_widgets.firmwareList = new QListWidget(this);
    m_widgets.firmwareList->addItem("x");
    m_widgets.firmwareInfo = new QTextBrowser(this);

    // icons for the right icon box
    m_widgets.pinButton = new QToolButton(this);
    m_widgets.pinButton->setIcon(QPixmap(":/chip.png"));

    // progress bar
    m_widgets.mainProgress = new QProgressBar(this);

    rightTopBoxLayout->addWidget(m_widgets.devicesLabel);
    rightTopBoxLayout->addWidget(m_widgets.devicesCombo);
    rightTopBoxLayout->addStretch(1);
    rightTopBoxLayout->addWidget(m_widgets.refreshButton);
    rightTopBoxLayout->addWidget(m_widgets.uploadButton);
    rightTopBoxLayout->setContentsMargins(0, 0, 0, 0);

    rightButtonBoxLayout->addStretch(1);
    rightButtonBoxLayout->addWidget(m_widgets.pinButton);
    rightButtonBoxLayout->setContentsMargins(0, 0, 0, 0);

    rightMiddleBoxLayout->addWidget(m_widgets.firmwareList);
    rightMiddleBoxLayout->setStretchFactor(m_widgets.firmwareList, 1);
    rightMiddleBoxLayout->addWidget(m_widgets.firmwareInfo);
    rightMiddleBoxLayout->setStretchFactor(m_widgets.firmwareInfo, 4);
    rightMiddleBoxLayout->addWidget(m_widgets.rightButtonBox);
    rightMiddleBoxLayout->setContentsMargins(0, 0, 0, 0);

    rightBoxLayout->addWidget(m_widgets.rightTopBox);
    rightBoxLayout->addWidget(m_widgets.rightMiddleBox);
    rightBoxLayout->addSpacing(5);
    rightBoxLayout->addWidget(m_widgets.mainProgress);

    //
    // main layout
    //

    mainWidgetLayout->addWidget(m_widgets.leftBox);
    mainWidgetLayout->addWidget(m_widgets.rightBox);

    setCentralWidget(m_widgets.mainWidget);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::refreshDevices()
{

}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
