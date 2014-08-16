/*
 * (c) 2010-2014, Bernhard Walle <bernhard@bwalle.de>
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
#include <QStackedWidget>
#include <QRadioButton>
#include <QLineEdit>

#include <usbprog-core/devices.h>
#include <usbprog/firmwarepool.h>

namespace usbprog {
namespace gui {

class UsbprogApplication;

/* MainWindowProgressNotifier {{{ */

/**
 * @class ProgressBarProgressNotifier gui/usbprog_mainwindow.h
 * @brief Notifier implementation for the GUI
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class ProgressBarProgressNotifier : public QObject, public core::ProgressNotifier
{
Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * Creates a new instance of a ProgressBarProgressNotifier object.
     *
     * @param[in] progressBar the progress bar that should be updated
     * @param[in] statusBar the status bar (for the finish message)
     */
    ProgressBarProgressNotifier(QProgressBar *progressBar, QStatusBar *statusBar);

public:
    /**
     * @brief Sets the status message
     *
     * Sets the status message that should be displayed when the operation has finished.
     *
     * @param[in] statusMessage the message string (must be already translated)
     */
    void setStatusMessage(const QString &statusMessage);

    /// @copydoc core::ProgressNotifier::progressed()
    int progressed(double total, double now);

    /// @copydoc core::ProgressNotifier::finished()
    void finished();

protected slots:
    /**
     * @brief Slot that reset the progress bar
     */
    void resetProgressbar();

private:
    QProgressBar *m_progressBar;
    QString m_statusMessage;
    QStatusBar *m_statusBar;
};

/* }}} */
/* UsbprogMainWindow {{{ */

/**
 * @class UsbprogMainWindow gui/usbprog_mainwindow.h
 * @brief Main window for the USBprog updater
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class UsbprogMainWindow : public QMainWindow
{
Q_OBJECT

public:
    /// Default timeout for messages.
    static const int DEFAULT_MESSAGE_TIMEOUT;

    /// File name for the PDF version of the User's Guide.
    static const char *USERS_GUIDE_FILENAME;

public:
    /**
     * @brief Constructor
     */
    UsbprogMainWindow(UsbprogApplication &app);

    /**
     * @brief Destructor
     */
    virtual ~UsbprogMainWindow();

protected:
    /**
     * @brief Initializes the actions
     */
    void initActions();

    /**
     * @brief Initializes the menus of the application
     */
    void initMenus();

    /**
     * @brief Initializes the widgets of the application
     */
    void initWidgets();

    /**
     * @brief Connects signals and slots.
     */
    void connectSignalsAndSlots();

    /**
     * @brief Initializes the firmwares.
     *
     * Downloads the index file and asks the firmware pool to parse the firmware pool.
     */
    void initFirmwares();

    /**
     * @brief Downloads a firmware
     *
     * @param[in] name the name of the firmware that should be downloaded.
     * @param[in] failSilent if set to @c true, don't display a status message if downloading of
     *            a specific firmware fails (this is meant to be used for the "download all"
     *            action because one wouldn't see the error message)
     * @return @c true if downloading succeeded, @c false if it bailed
     */
    bool downloadFirmware(const std::string &name, bool failSilent=false);

public slots:
    /**
     * @brief Refresh the device combo box
     */
    void refreshDevices();

    /**
     * @brief Update the information for the selected firmware
     *
     * @param[in] newItem the firmware item that has been selected by the user
     */
    void firmwareSelected(QListWidgetItem *newItem);

    /**
     * @brief Uploads the selected firmware to the selected device
     */
    void uploadFirmware();

    /**
     * @brief Shows the help
     */
    void showHelp();

    /**
     * @brief Shows the about dialog
     */
    void showAbout();

    /**
     * @brief Cleans the cache.
     */
    void cacheClean();

    /**
     * @brief Deletes the cache.
     */
    void cacheDelete();

    /**
     * @brief Downloads all firmwares.
     */
    void cacheDownloadAll();

    /**
     * @brief Displays the pin assignment dialog.
     */
    void showPinDialog();

    /**
     * @brief Enables debugging
     *
     * @param[in] @c true if debugging should be enabled, @c false otherwise.
     */
    void enableDebugging(bool enabled);

    /**
     * @brief Called when the online pool radio button is toggled
     *
     * @param[in] enabled @c true if the online pool is enabled, @c false if the file is enabled
     */
    void onlinePoolSourceToggled(bool enabled);

    /**
     * @brief Allows selection of a firmware file
     */
    void fileChooseButtonClicked();

private:
    core::DeviceManager *m_deviceManager;
    Firmwarepool *m_firmwarepool;
    ProgressBarProgressNotifier *m_progressNotifier;

    struct {
        // container widgets
        QWidget        *mainWidget;
        QWidget        *leftBox;
        QWidget        *rightBox;
        QWidget        *rightTopBox;
        QWidget        *firmwareFileBox;
        QWidget        *firmwareSourceBox;
        QWidget        *rightButtonBox;
        QWidget        *firmwarePoolBox;
        QStackedWidget *firmwareStack;
        // the real widgets
        QLabel         *usbprogGraphicsLabel;
        QLabel         *websiteLinkLabel;
        QComboBox      *devicesCombo;
        QPushButton    *refreshButton;
        QPushButton    *uploadButton;
        QListWidget    *firmwareList;
        QTextBrowser   *firmwareInfo;
        QToolButton    *pinButton;
        QProgressBar   *mainProgress;
        QLabel         *firmwareSourceLabel;
        QRadioButton   *firmwareSourcePoolRadio;
        QRadioButton   *firmwareSourceFileRadio;
        QLineEdit      *fileEdit;
        QPushButton    *fileChooseButton;
    } m_widgets;

    struct {
        QAction      *logging;
        QAction      *quit;
        QAction      *help;
        QAction      *aboutQt;
        QAction      *about;
        QAction      *cacheDelete;
        QAction      *cacheClean;
        QAction      *cacheDownloadAll;
    } m_actions;

    UsbprogApplication &m_app;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog

#endif // USBPROG_MAINWINDOW_H

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
