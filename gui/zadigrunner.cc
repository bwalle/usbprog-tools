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

#include <QSysInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QDebug>
#include <QTextStream>

#include <usbprog/downloader.h>
#include <usbprog-core/debug.h>

#ifdef Q_OS_WIN
#  include <windows.h>
#  include <shellapi.h>
#endif

#include "zadigrunner.h"

#define ZADIG_URL_VISTA "http://zadig.akeo.ie/downloads/zadig.exe"
#define ZADIG_URL_XP    "http://zadig.akeo.ie/downloads/zadig_xp.exe"
#define ZADIG_FILE_NAME "zadig.exe"

namespace usbprog {
namespace gui {

ZadigRunner::ZadigRunner(QObject *parent) :
    QObject(parent),
    m_tempdir()
{
    m_tempdir.setAutoRemove(true);
}

ZadigRunner::~ZadigRunner()
{}

void ZadigRunner::startDownload()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(Downloader::createRequest(downloadUrl()));

    QNetworkReply *reply(manager->get(request));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(downloadProgressSlot(qint64,qint64)));
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadFinishedSlot(QNetworkReply*)));
}

bool ZadigRunner::startTool()
{
    QString exeName = filenameInTemp("zadig.exe");

#ifdef Q_OS_WIN
    QString workingDir = QDir::currentPath();

    // change working directory
    if (!QDir::setCurrent(m_tempdir.path()))
        qWarning() << "Unable to set working directory to " << m_tempdir.path();

    int result = (int)::ShellExecuteA(0, "open", exeName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
    if (result == SE_ERR_ACCESSDENIED) {
        // Requesting elevation
        result = (int)::ShellExecuteA(0, "runas", exeName.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
    }

    if (!QDir::setCurrent(workingDir))
        qWarning() << "Unable to restore working directory to " << workingDir;

    if (result <= 32) {
        USBPROG_DEBUG_DBG("Windows error code for running zadig.exe: %d", result);
        return false;
    }

    return true;
#else
    USBPROG_DEBUG_INFO("Unable to start zadig.exe on non-Windows operating systems.");
    return false;
#endif
}

std::string ZadigRunner::downloadUrl() const
{
#ifdef Q_OS_WIN
    if (QSysInfo::windowsVersion() < QSysInfo::WV_XP)
        return ZADIG_URL_XP;
    else
        return ZADIG_URL_VISTA;
#else
    // for testing only
    return ZADIG_URL_VISTA;
#endif
}

QString ZadigRunner::filenameInTemp(const QString &name) const
{
    return QDir(m_tempdir.path()).absoluteFilePath(name);
}

bool ZadigRunner::generateConfigurationFiles()
{
    QFile zadigIniFile(filenameInTemp("zadig.ini"));
    if (!zadigIniFile.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream zadigIniStream(&zadigIniFile);
    zadigIniStream
        << "# Sample ini file for Zadig\n"
        << "[general]\n"
        << "# Start application in advanced mode (default = false)\n"
        << "advanced_mode = false\n"
        << "# Exit application upon successful driver installation (default = false)\n"
        << "exit_on_success = true\n"
        << "# Log level (0=debug, 1=info, 2=warning, 3=error)\n"
        << "log_level = 0\n"
        << "\n"
        << "[device]\n"
        << "# List all devices, including the ones that already have a driver (default = false)\n"
        << "list_all = false\n"
        << "# Include hubs and composite parent devices when listing all (default = false)\n"
        << "#include_hubs = true\n"
        << "# Trim trailing whitespaces from the USB device description (default = false)\n"
        << "#trim_whitespaces = true\n"
        << "\n"
        << "[driver]\n"
        << "# Select the following as the default driver to install:\n"
        << "# WinUSB = 0, libusb0.sys = 1, libusbK.sys = 2, Custom = 3 (default = WinUSB)\n"
        << "default_driver = 1\n"
        << "# Extract driver files only, don't install (default = false)\n"
        << "extract_only = false\n";
    zadigIniFile.close();

    QFile usbprogCfgFile(filenameInTemp("usbprog.cfg"));
    if (!usbprogCfgFile.open(QFile::WriteOnly | QFile::Truncate))
        return false;

    QTextStream usbprogCfgStream(&usbprogCfgFile);
    usbprogCfgStream
        << "[device]\n"
        << "Description = \"USBprog in update mode\"\n"
        << "VID = 0x1781\n"
        << "PID = 0x0C62\n";

    usbprogCfgFile.close();

    return true;
}

void ZadigRunner::downloadProgressSlot(qint64 received, qint64 total)
{
    USBPROG_DEBUG_TRACE("Got download progress (%llu/%llu)", received, total);
    if (total != 0)
        emit downloadProgress(received*100/total);
}

void ZadigRunner::downloadFinishedSlot(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit downloadError(reply->errorString());
        return;
    }

    QString zadigExe = filenameInTemp("zadig.exe");

    QFile outputFile(zadigExe);
    if (!outputFile.open(QFile::WriteOnly)) {
        emit downloadError(tr("Unable to open output file '%1' for writing").arg(zadigExe));
        return;
    }

    QByteArray data = reply->readAll();
    if (outputFile.write(data) != data.size()) {
        emit downloadError(tr("Unable to write %1 bytes to %2").arg(data.size()).arg(zadigExe));
        return;
    }

    qDebug() << "Zadig downloaded to " << zadigExe;

    if (!generateConfigurationFiles()) {
        emit downloadError(tr("Unable to generate Zadig configuration files"));
        return;
    }

    emit downloadFinished();
}

} // end namespace gui
} // end namespace usbprog
