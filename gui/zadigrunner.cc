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

#include <usbprog/downloader.h>
#include <usbprog-core/debug.h>

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
#if 0
    connect(reply, SIGNAL())

    USBPROG_DEBUG_DBG("Performing download");
    while (!m_finished) {
        QByteArray readData = reply->readAll();
        m_output.write(readData.constData(), readData.size());
        qApp->processEvents(QEventLoop::WaitForMoreEvents, 500);
    }

    QByteArray readData = reply->readAll();
    m_output.write(readData.constData(), readData.size());

    if (m_notifier)
        m_notifier->finished();

    if (reply->error() != QNetworkReply::NoError)
        throw DownloadError(static_cast<const char *>(reply->errorString().toUtf8()));
#endif

}

bool ZadigRunner::startTool()
{

    return true;
}

std::string ZadigRunner::downloadUrl() const
{
    if (QSysInfo::windowsVersion() < QSysInfo::WV_XP)
        return ZADIG_URL_XP;
    else
        return ZADIG_URL_VISTA;
}

QString ZadigRunner::zadigFileName() const
{
    return QDir(m_tempdir.path()).absoluteFilePath("zadig.exe");
}

void ZadigRunner::downloadProgressSlot(qint64 received, qint64 total)
{
    USBPROG_DEBUG_TRACE("Got download progress (%llu/%llu)", received, total);
    if (total != 0)
        emit downloadProgress(received*100/total);
}

void ZadigRunner::downloadFinishedSlot(QNetworkReply *reply)
{
    QString filename = zadigFileName();

    QFile outputFile(filename);
    if (!outputFile.open(QFile::WriteOnly))
        emit downloadError(tr("Unable to open output file '%1' for writing").arg(filename));

    QByteArray data = reply->readAll();
    if (outputFile.write(data) != data.size())
        emit downloadError(tr("Unable to write %1 bytes to %2").arg(data.size()).arg(filename));

    emit downloadFinished();
}

} // end namespace gui
} // end namespace usbprog
