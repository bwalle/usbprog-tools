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
#include <cassert>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QTimer>
#include <QTextStream>

#include <usbprog-core/debug.h>

#include "usbprog_mainwindow.h"
#include "usbprog_app.h"
#include "guiconfiguration.h"
#include "qtsleeper.h"

/* ProgressBarProgressNotifier {{{ */

ProgressBarProgressNotifier::ProgressBarProgressNotifier(QProgressBar *progressBar, QStatusBar *statusBar)
    : m_progressBar(progressBar)
    , m_statusBar(statusBar)
{}

void ProgressBarProgressNotifier::setStatusMessage(const QString &statusMessage)
{
    m_statusMessage = statusMessage;
}

int ProgressBarProgressNotifier::progressed(double total, double now)
{
    m_progressBar->setValue(now*1000);
    m_progressBar->setMaximum(total*1000);
}

void ProgressBarProgressNotifier::finished()
{
    m_progressBar->setValue(1000);
    m_progressBar->setMaximum(1000);

    QTimer::singleShot(UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT, this, SLOT(resetProgressbar()));
    if (m_statusBar && !m_statusMessage.isNull())
        m_statusBar->showMessage(m_statusMessage, UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
}

void ProgressBarProgressNotifier::resetProgressbar()
{
    m_progressBar->reset();
}

/* }}} */
/* UsbprogMainWindow {{{ */

// -----------------------------------------------------------------------------
const int UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT = 2000;

// -----------------------------------------------------------------------------
UsbprogMainWindow::UsbprogMainWindow()
    : m_deviceManager(NULL)
    , m_firmwarepool(NULL)
    , m_progressNotifier(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);

    std::memset(&m_widgets, 0, sizeof(m_widgets));
    std::memset(&m_actions, 0, sizeof(m_actions));

    initActions();
    initWidgets();
    initMenus();
    connectSignalsAndSlots();

    setWindowIcon(QPixmap(":/usbprog_icon.xpm"));
    setWindowTitle(UsbprogApplication::NAME);

    m_deviceManager = new DeviceManager;
    m_deviceManager->setCustomSleeper(new QtSleeper);
    m_firmwarepool = new Firmwarepool(GuiConfiguration::config().getDataDir());
    m_progressNotifier = new ProgressBarProgressNotifier(m_widgets.mainProgress, statusBar());
    m_firmwarepool->setProgress(m_progressNotifier);

    // intially populate the device and the firmware list
    initFirmwares();
    refreshDevices();

    m_widgets.firmwareList->setFocus();
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
#if QT_VERSION < 0x040600
    m_actions.quit->setShortcut(Qt::CTRL | Qt::Key_Q);
#else
    m_actions.quit->setShortcut(QKeySequence::Quit);
#endif
    m_actions.quit->setStatusTip(tr("Quits the program"));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::connectSignalsAndSlots()
{
    connect(m_widgets.refreshButton, SIGNAL(clicked()), SLOT(refreshDevices()));
    connect(m_actions.quit, SIGNAL(activated()), SLOT(close()));

    connect(m_widgets.firmwareList,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            SLOT(firmwareSelected(QListWidgetItem *)));

    connect(m_widgets.uploadButton, SIGNAL(clicked()), SLOT(uploadFirmware()));
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

    // devices combo box
    m_widgets.devicesCombo = new QComboBox(this);
    m_widgets.devicesCombo->setSizeAdjustPolicy(QComboBox::AdjustToContents);

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
    m_widgets.firmwareInfo = new QTextBrowser(this);
    m_widgets.firmwareInfo->setMinimumHeight(300);

    // icons for the right icon box
    m_widgets.pinButton = new QToolButton(this);
    m_widgets.pinButton->setIcon(QPixmap(":/chip.png"));

    // progress bar
    m_widgets.mainProgress = new QProgressBar(this);
    m_widgets.mainProgress->setMaximumHeight(10);
    m_widgets.mainProgress->setTextVisible(false);

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
void UsbprogMainWindow::initFirmwares()
{
    GuiConfiguration &conf = GuiConfiguration::config();

    // init the firmware pool and download firmwares first
    try {
        m_progressNotifier->setStatusMessage(tr("Downloading of firmware index finished."));
        m_firmwarepool->setIndexUpdatetime(AUTO_NOT_UPDATE_TIME);
        if (!conf.isOffline())
            m_firmwarepool->downloadIndex(conf.getIndexUrl());
        m_firmwarepool->readIndex();
    } catch (const std::runtime_error &re) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Error while downloading firmware index:\n\n%1").arg(re.what()));
        return;
    }

    StringList firmwareNames = m_firmwarepool->getFirmwareNameList();
    for (StringList::iterator it = firmwareNames.begin(); it != firmwareNames.end(); ++it)
        m_widgets.firmwareList->addItem(QString::fromStdString(*it));

    // select first item
    if (!firmwareNames.empty())
        m_widgets.firmwareList->setCurrentItem(m_widgets.firmwareList->item(0));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::refreshDevices()
{
    m_deviceManager->discoverUpdateDevices(m_firmwarepool->getUpdateDeviceList());

    m_widgets.devicesCombo->clear();
    m_widgets.devicesCombo->addItem(tr("No device"), int(-1));


    if (m_deviceManager->getNumberUpdateDevices() == 0) {
        statusBar()->showMessage(tr("No devices found."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        return;
    }

    // build the device list
    for (size_t i = 0; i < m_deviceManager->getNumberUpdateDevices(); ++i) {
        Device *dev = m_deviceManager->getDevice(i);
        m_widgets.devicesCombo->addItem(QString::fromStdString(dev->toShortString()), int(i));
    }
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::firmwareSelected(QListWidgetItem *newItem)
{
    Debug::debug()->dbg("Firmware '%s' selected", static_cast<const char *>(newItem->text().toLocal8Bit()));

    Firmware *fw = m_firmwarepool->getFirmware(newItem->text().toStdString());

    QString html;
    QTextStream htmlStream(&html);

    // start HTML
    htmlStream << "<html><body><table border=0>\n";

    // Name
    htmlStream << "<tr><td align=\"right\"><b>Name:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->getLabel())
               << "</td> </tr>\n";

    // URL
    htmlStream << "<tr><td align=\"right\"><b>URL:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->getUrl())
               << "</td> </tr>\n";

#if 0
    // file name
    htmlStream << "<tr><td align=\"right\"><b>File name:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->getFilename())
               << "</td> </tr>\n";
#endif

    // version
    htmlStream << "<tr><td align=\"right\"><b>Version:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->getVersionString()) << " "
               << "[" << QString::fromStdString(fw->getDate().getDateTimeString(DTF_ISO_DATE)) << "]"
               << "</td> </tr>\n";

    // device IDs
    htmlStream << "<tr><td align=\"right\"><b>Device IDs:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->updateDevice().formatDeviceId())
               << "</td> </tr>\n";

    // offline?
    htmlStream << "<tr><td align=\"right\"><b>Offline:</b></td> <td>&nbsp;</td> <td>"
               << QString(m_firmwarepool->isFirmwareOnDisk(fw->getName())
                          ? QChar(0x2713)
                          : QChar(0x2717))
               << "</td> </tr>\n";

    htmlStream << "</table>";

    // description
    htmlStream << "<p><tt style=\""
#ifdef Q_WS_X11
               << "font-family: Monospace;"
#endif
               << "\">"
               << QString::fromStdString(fw->getDescription()) << "</tt></p>";

    // end HTML
    htmlStream << "</html>";

    m_widgets.firmwareInfo->setHtml(html);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::uploadFirmware()
{
    QList<QListWidgetItem *> selectedItems = m_widgets.firmwareList->selectedItems();
    if (selectedItems.size() != 1) {
        statusBar()->showMessage(tr("No firmwares selected."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        return;
    }
    Firmware *fw = m_firmwarepool->getFirmware(selectedItems.front()->text().toStdString());

    int deviceIndex = m_widgets.devicesCombo->itemData(m_widgets.devicesCombo->currentIndex()).toInt();
    if (deviceIndex == -1) {
        statusBar()->showMessage(tr("No update device selected."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        return;
    }

    m_deviceManager->setCurrentUpdateDevice(deviceIndex);
    Device *updateDevice = m_deviceManager->getCurrentUpdateDevice();
    assert(updateDevice != NULL);

    Debug::debug()->dbg("Uploading firmware '%s' to '%s'", fw->getName().c_str(), updateDevice->toShortString().c_str());

    // download firmware if necessary
    if (!downloadFirmware(fw->getName()))
        return;

    // switch in update mode
    if (!updateDevice->isUpdateMode()) {
        try {
            statusBar()->showMessage(tr("Switching to update mode ..."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
            m_deviceManager->switchUpdateMode();
        } catch (const IOError &err) {
            QMessageBox::critical(this, UsbprogApplication::NAME,
                                  tr("I/O error while switching to update mode:\n\n%1").arg(err.what()));
            return;
        }
    }

    updateDevice = m_deviceManager->getCurrentUpdateDevice();
    if (!updateDevice) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Unable to find the update device after switching to update mode."));
        return;
    }
    UsbprogUpdater updater(updateDevice);

    try {
        m_progressNotifier->setStatusMessage(QString());
        updater.setProgress(m_progressNotifier);

        Debug::debug()->dbg("Opening device");
        statusBar()->showMessage(tr("Opening device ..."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        updater.updateOpen();

        Debug::debug()->dbg("Writing firmware");
        statusBar()->showMessage(tr("Writing firmware ..."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        updater.writeFirmware(fw->getData());

        Debug::debug()->dbg("Starting device");
        statusBar()->showMessage(tr("Starting device ..."), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
        updater.startDevice();

        Debug::debug()->dbg("Closing updater");
        updater.updateClose();
    } catch (const IOError &err) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Unable to write the firmware:\n\n%1").arg(err.what()));
        return;
    }

    statusBar()->showMessage(tr("Firmware uploaded sucessfully!"), UsbprogMainWindow::DEFAULT_MESSAGE_TIMEOUT);
    QTimer::singleShot(2000, this, SLOT(refreshDevices()));
}

// -----------------------------------------------------------------------------
bool UsbprogMainWindow::downloadFirmware(const std::string &name)
{
    QString qName = QString::fromStdString(name);
    if (!m_firmwarepool->isFirmwareOnDisk(name)) {
        Debug::debug()->dbg("Firmware '%s' not on disk, downloading...", name.c_str());
        statusBar()->showMessage(tr("Downloading firmware %1 ...").arg(qName), DEFAULT_MESSAGE_TIMEOUT);

        try {
            m_progressNotifier->setStatusMessage(tr("Downloading firmware %1 finished.").arg(QString::fromStdString(name)));
            m_firmwarepool->downloadFirmware(name);
        } catch (const DownloadError &err) {
            QMessageBox::critical(this, UsbprogApplication::NAME,
                                  tr("I/O error while downloading firmware \"%1\":\n\n%2")
                                  .arg(qName).arg(err.what()) );
            return false;
        }
    }

    try {
        m_firmwarepool->fillFirmware(name);
    } catch (const IOError &err) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("I/O error while filling firmware \"%1\":\n\n%2")
                              .arg(qName).arg(err.what()) );
        return false;
    }

    return true;
}

/* }}} */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
