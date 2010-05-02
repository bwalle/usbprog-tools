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
#ifndef USBPROG_MAINWINDOW_H
#define USBPROG_MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QProgressBar>
#include <QComboBox>
#include <QListWidget>
#include <QTextBrowser>

#include <usbprog-core/devices.h>
#include <usbprog/firmwarepool.h>

namespace usbprog {
namespace gui {

/* MainWindowProgressNotifier {{{ */

class ProgressBarProgressNotifier : public QObject, public core::ProgressNotifier
{
    Q_OBJECT

    public:
        ProgressBarProgressNotifier(QProgressBar *mainWindow, QStatusBar *statusBar);

    public:
        void setStatusMessage(const QString &statusMessage);
        int progressed(double total, double now);
        void finished();

    protected slots:
        void resetProgressbar();

    private:
        QProgressBar *m_progressBar;
        QString m_statusMessage;
        QStatusBar *m_statusBar;
};

/* }}} */
/* UsbprogMainWindow {{{ */

class UsbprogMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        static const int DEFAULT_MESSAGE_TIMEOUT;
        static const char *USERS_GUIDE_FILENAME;

    public:
        UsbprogMainWindow();
        virtual ~UsbprogMainWindow();

    protected:
        void initActions();
        void initMenus();
        void initWidgets();
        void connectSignalsAndSlots();
        void initFirmwares();
        bool downloadFirmware(const std::string &name, bool failSilent=false);

    public slots:
        void refreshDevices();
        void firmwareSelected(QListWidgetItem *newItem);
        void uploadFirmware();
        void showHelp();
        void showAbout();
        void cacheClean();
        void cacheDelete();
        void cacheDownloadAll();

    private:
        core::DeviceManager *m_deviceManager;
        Firmwarepool *m_firmwarepool;
        ProgressBarProgressNotifier *m_progressNotifier;

        struct {
            // container widgets
            QWidget      *mainWidget;
            QWidget      *leftBox;
            QWidget      *rightBox;
            QWidget      *rightTopBox;
            QWidget      *rightButtonBox;
            QWidget      *rightMiddleBox;
            // the real widgets
            QLabel       *usbprogGraphicsLabel;
            QLabel       *websiteLinkLabel;
            QComboBox    *devicesCombo;
            QPushButton  *refreshButton;
            QPushButton  *uploadButton;
            QListWidget  *firmwareList;
            QTextBrowser *firmwareInfo;
            QToolButton  *pinButton;
            QProgressBar *mainProgress;
        } m_widgets;

        struct {
            QAction      *quit;
            QAction      *help;
            QAction      *aboutQt;
            QAction      *about;
            QAction      *cacheDelete;
            QAction      *cacheClean;
            QAction      *cacheDownloadAll;
        } m_actions;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // USBPROG_MAINWINDOW_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
