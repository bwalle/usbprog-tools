/*
 * (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
#include <stdexcept>
#include <ostream>
#include <cstring>
#include <memory>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QCoreApplication>

#include <usbprog-core/debug.h>
#include <usbprog/sysinfo.h>
#include <usbprog/downloader.h>
#include <usbprog/usbprog.h>

#include "config.h"

namespace usbprog {

/* Members {{{ */

/* -------------------------------------------------------------------------- */
Downloader::Downloader(std::ostream &output)
    : m_notifier(NULL)
    , m_output(output)
{}

/* -------------------------------------------------------------------------- */
QNetworkRequest Downloader::createRequest(const std::string &url)
{
    std::string userAgent("USBprog/" USBPROG_VERSION_STRING);
    userAgent += " on " + Sysinfo::osName() + " " + Sysinfo::osVersion();

    QNetworkRequest request(QUrl(url.c_str()));
    request.setRawHeader("User-Agent", userAgent.c_str());
    USBPROG_DEBUG_DBG("Setting 'User-Agent' header to '%s'", userAgent.c_str());

    return request;
}

/* -------------------------------------------------------------------------- */
void Downloader::setUrl(const std::string &url)
{
    USBPROG_DEBUG_DBG("Setting URL to '%s'", url.c_str());
    m_url = url;
}

/* -------------------------------------------------------------------------- */
std::string Downloader::getUrl() const
{
    return m_url;
}

/* -------------------------------------------------------------------------- */
void Downloader::setProgress(core::ProgressNotifier *notifier)
{
    m_notifier = notifier;
}

/* -------------------------------------------------------------------------- */
void Downloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (m_notifier)
        m_notifier->progressed(bytesTotal, bytesReceived);
}

/* -------------------------------------------------------------------------- */
void Downloader::downloadFinished()
{
    m_finished = true;
}

/* -------------------------------------------------------------------------- */
void Downloader::download()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(createRequest(m_url));
    m_finished = false;

    QNetworkReply *reply(manager->get(request));
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)),
            SLOT(downloadProgress(qint64, qint64)));
    connect(reply, SIGNAL(finished()), SLOT(downloadFinished()));

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
}

/* }}} */

} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
