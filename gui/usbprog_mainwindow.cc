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
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDesktopServices>

#include <usbprog-core/debug.h>
#include <usbprog-core/util.h>

#include "usbprog_mainwindow.h"
#include "usbprog_app.h"
#include "guiconfiguration.h"
#include "qtsleeper.h"
#include "config.h"

namespace usbprog {
namespace gui {

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
    return true;
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
const char *UsbprogMainWindow::USERS_GUIDE_FILENAME = "USBprog.pdf";

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

    m_deviceManager = new core::DeviceManager;
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

    m_actions.help = new QAction(QIcon(":/gtk-help.png"), tr("&Show Manual"), this);
    m_actions.help->setShortcut(QKeySequence::HelpContents);
    m_actions.help->setStatusTip(tr("Opens a PDF viewer with the \"User's Manual\""));

    m_actions.aboutQt = new QAction(tr("&About Qt..."), this);
    m_actions.aboutQt->setStatusTip(tr("Shows the Qt version."));

    m_actions.about = new QAction(QIcon(":/gtk-about.png"), tr("&About..."), this);
    m_actions.about->setStatusTip(tr("Shows author and version number of that application."));

    m_actions.cacheClean = new QAction(QIcon(":/gtk-clear.png"), tr("&Clean unused files"), this);
    m_actions.cacheClean->setStatusTip(tr("Deletes obsolete files in firmware the cache."));

    m_actions.cacheDelete = new QAction(QIcon(":/gtk-delete.png"), tr("&Delete files"), this);
    m_actions.cacheDelete->setStatusTip(tr("Deletes all cached firmware files."));

    m_actions.cacheDownloadAll = new QAction(tr("Download &all"), this);
    m_actions.cacheDownloadAll->setStatusTip(tr("Downloads all firmware files to have them locally available."));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::connectSignalsAndSlots()
{
    connect(m_widgets.refreshButton, SIGNAL(clicked()), SLOT(refreshDevices()));
    connect(m_actions.quit, SIGNAL(activated()), SLOT(close()));
    connect(m_actions.help, SIGNAL(activated()), SLOT(showHelp()));
    connect(m_actions.aboutQt, SIGNAL(activated()), qApp, SLOT(aboutQt()));
    connect(m_actions.about, SIGNAL(activated()), SLOT(showAbout()));
    connect(m_actions.cacheClean, SIGNAL(activated()), SLOT(cacheClean()));
    connect(m_actions.cacheDelete, SIGNAL(activated()), SLOT(cacheDelete()));
    connect(m_actions.cacheDownloadAll, SIGNAL(activated()), SLOT(cacheDownloadAll()));

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

    QMenu *cacheMenu = new QMenu(this);
    cacheMenu->setTitle(tr("&Cache"));
    cacheMenu->addAction(m_actions.cacheClean);
    cacheMenu->addAction(m_actions.cacheDelete);
    cacheMenu->addSeparator();
    cacheMenu->addAction(m_actions.cacheDownloadAll);

    QMenu *helpMenu = new QMenu(this);
    helpMenu->setTitle(tr("&Help"));
    helpMenu->addAction(m_actions.help);
    helpMenu->addSeparator();
    helpMenu->addAction(m_actions.aboutQt);
    helpMenu->addAction(m_actions.about);

    menuBar()->addMenu(programMenu);
    menuBar()->addMenu(cacheMenu);
    // align the "Help" menu on the right in the Motif and CDE style
    menuBar()->addSeparator();
    menuBar()->addMenu(helpMenu);
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
    m_widgets.firmwareInfo->setOpenExternalLinks(true);

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
        statusBar()->showMessage(tr("No devices found."), DEFAULT_MESSAGE_TIMEOUT);
        return;
    }

    // build the device list
    for (size_t i = 0; i < m_deviceManager->getNumberUpdateDevices(); ++i) {
        core::Device *dev = m_deviceManager->getDevice(i);
        m_widgets.devicesCombo->addItem(QString::fromStdString(dev->toShortString()), int(i));
    }
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::firmwareSelected(QListWidgetItem *newItem)
{
    if (!newItem)
        newItem = m_widgets.firmwareList->currentItem();

    USBPROG_DEBUG_DBG("Firmware '%s' selected", static_cast<const char *>(newItem->text().toLocal8Bit()));

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
               << "<a target=\"_top\" href=\"" << QString::fromStdString(fw->getUrl()) << "\">" 
               << QString::fromStdString(fw->getUrl()) << "</a>"
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
               << "[" << QString::fromStdString(fw->getDate().getDateTimeString(core::DTF_ISO_DATE)) << "]"
               << "</td> </tr>\n";

    // device IDs
    htmlStream << "<tr><td align=\"right\"><b>Device IDs:</b></td> <td>&nbsp;</td> <td>"
               << QString::fromStdString(fw->updateDevice().formatDeviceId())
               << "</td> </tr>\n";

    // cached?
    htmlStream << "<tr><td align=\"right\"><b>Cached:</b></td> <td>&nbsp;</td> <td>"
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
        statusBar()->showMessage(tr("No firmwares selected."), DEFAULT_MESSAGE_TIMEOUT);
        return;
    }
    Firmware *fw = m_firmwarepool->getFirmware(selectedItems.front()->text().toStdString());

    int deviceIndex = m_widgets.devicesCombo->itemData(m_widgets.devicesCombo->currentIndex()).toInt();
    if (deviceIndex == -1) {
        statusBar()->showMessage(tr("No update device selected."), DEFAULT_MESSAGE_TIMEOUT);
        return;
    }

    m_deviceManager->setCurrentUpdateDevice(deviceIndex);
    core::Device *updateDevice = m_deviceManager->getCurrentUpdateDevice();
    assert(updateDevice != NULL);

    USBPROG_DEBUG_DBG("Uploading firmware '%s' to '%s'", fw->getName().c_str(), updateDevice->toShortString().c_str());

    // download firmware if necessary
    if (!downloadFirmware(fw->getName()))
        return;

    // switch in update mode
    if (!updateDevice->isUpdateMode()) {
        try {
            statusBar()->showMessage(tr("Switching to update mode ..."), DEFAULT_MESSAGE_TIMEOUT);
            m_deviceManager->switchUpdateMode();
        } catch (const core::IOError &err) {
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
    core::UsbprogUpdater updater(updateDevice);

    try {
        m_progressNotifier->setStatusMessage(QString());
        updater.setProgress(m_progressNotifier);

        USBPROG_DEBUG_DBG("Opening device");
        statusBar()->showMessage(tr("Opening device ..."), DEFAULT_MESSAGE_TIMEOUT);
        updater.updateOpen();

        USBPROG_DEBUG_DBG("Writing firmware");
        statusBar()->showMessage(tr("Writing firmware ..."), DEFAULT_MESSAGE_TIMEOUT);
        updater.writeFirmware(fw->getData());

        USBPROG_DEBUG_DBG("Starting device");
        statusBar()->showMessage(tr("Starting device ..."), DEFAULT_MESSAGE_TIMEOUT);
        updater.startDevice();

        USBPROG_DEBUG_DBG("Closing updater");
        updater.updateClose();
    } catch (const core::IOError &err) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Unable to write the firmware:\n\n%1").arg(err.what()));
        return;
    }

    statusBar()->showMessage(tr("Firmware uploaded sucessfully!"), DEFAULT_MESSAGE_TIMEOUT);
    QTimer::singleShot(2000, this, SLOT(refreshDevices()));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::showHelp()
{
    USBPROG_DEBUG_DBG("Open help (docdir=%s)", DOCDIR);

    QString fileName, localPath, globalPath;
    localPath = QString::fromStdString(core::pathconcat("doc", USERS_GUIDE_FILENAME));
    globalPath = QString::fromStdString(core::pathconcat(DOCDIR, USERS_GUIDE_FILENAME));
    if (QFile::exists(localPath))
        fileName = localPath;
    else if (QFile::exists(globalPath))
        fileName = globalPath;

    if (fileName.isNull()) {
        statusBar()->showMessage(tr("\"User's Manual\" not found."), DEFAULT_MESSAGE_TIMEOUT);
        return;
    }

    QString absolutePath = QFileInfo(fileName).canonicalFilePath();
    USBPROG_DEBUG_DBG("Help found at '%s'", static_cast<const char *>(absolutePath.toLocal8Bit()) );

    if (QDesktopServices::openUrl( QUrl::fromLocalFile(absolutePath)) )
        statusBar()->showMessage(tr("PDF viewer successfully started."), DEFAULT_MESSAGE_TIMEOUT);
    else
        statusBar()->showMessage(tr("Unable to open PDF viewer."), DEFAULT_MESSAGE_TIMEOUT);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::showAbout()
{
    USBPROG_DEBUG_DBG("Open about dialog");

    QMessageBox::information(this, UsbprogApplication::NAME,
                             tr("USBprog %1\n\n(c) 2007-2010 Bernhard Walle <bernhard@bwalle.de>")
                             .arg(USBPROG_VERSION_STRING));
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::cacheClean()
{
    USBPROG_DEBUG_DBG("Cleanup cache");

    try {
        m_firmwarepool->cleanCache();
    } catch (const core::IOError &ioe) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Error while cleaning up the cache:\n\n%1").arg(ioe.what()));
        return;
    }

    statusBar()->showMessage(tr("Cache successfully cleaned."), DEFAULT_MESSAGE_TIMEOUT);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::cacheDelete()
{
    USBPROG_DEBUG_DBG("Delete cache");

    try {
        m_firmwarepool->deleteCache();
    } catch (const core::IOError &ioe) {
        QMessageBox::critical(this, UsbprogApplication::NAME,
                              tr("Error while deleting the cache:\n\n%1").arg(ioe.what()));
        return;
    }

    statusBar()->showMessage(tr("Cache successfully deleted."), DEFAULT_MESSAGE_TIMEOUT);

    // update 'downloaded' status of currently selected firmware
    firmwareSelected(NULL);
}

// -----------------------------------------------------------------------------
void UsbprogMainWindow::cacheDownloadAll()
{
    USBPROG_DEBUG_DBG("Download all");

    bool someFail = false;
    bool someSuccess = false;

    StringList firmwares = m_firmwarepool->getFirmwareNameList();
    for (StringList::iterator it = firmwares.begin(); it != firmwares.end(); ++it)
        if (!downloadFirmware(*it, true))
            someFail = true;
        else
            someSuccess = true;

    if (someFail && someSuccess) {
        statusBar()->showMessage(tr("Some firmware files failed to download."), DEFAULT_MESSAGE_TIMEOUT);
    } else if (someFail && !someSuccess) {
        QMessageBox::information(this, UsbprogApplication::NAME,
                                 tr("Unable to download firmware files. Check network connection."));
    } else {
        statusBar()->showMessage(tr("Downloaded all firmwares successfully."), DEFAULT_MESSAGE_TIMEOUT);
    }
}

// -----------------------------------------------------------------------------
bool UsbprogMainWindow::downloadFirmware(const std::string &name, bool failSilent)
{
    QString qName = QString::fromStdString(name);
    if (!m_firmwarepool->isFirmwareOnDisk(name)) {
        USBPROG_DEBUG_DBG("Firmware '%s' not on disk, downloading...", name.c_str());
        statusBar()->showMessage(tr("Downloading firmware %1 ...").arg(qName), DEFAULT_MESSAGE_TIMEOUT);

        try {
            m_progressNotifier->setStatusMessage(tr("Downloading firmware %1 finished.").arg(QString::fromStdString(name)));
            m_firmwarepool->downloadFirmware(name);
        } catch (const DownloadError &err) {
            if (!failSilent) {
                QMessageBox::critical(this, UsbprogApplication::NAME,
                                      tr("I/O error while downloading firmware \"%1\":\n\n%2")
                                      .arg(qName).arg(err.what()) );
            }
            return false;
        }
    }

    try {
        m_firmwarepool->fillFirmware(name);
    } catch (const core::IOError &err) {
        if (!failSilent) {
            QMessageBox::critical(this, UsbprogApplication::NAME,
                                  tr("I/O error while filling firmware \"%1\":\n\n%2")
                                  .arg(qName).arg(err.what()) );
        }
        return false;
    }

    firmwareSelected(NULL);

    return true;
}

/* }}} */

} // end namespace gui
} // end namespace usbprog


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
