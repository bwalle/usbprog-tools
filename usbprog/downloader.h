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
#ifndef USBPROG_DOWNLOADER_H
#define USBPROG_DOWNLOADER_H

#include <stdexcept>
#include <ostream>

#include <QObject>

#include <usbprog/usbprog.h>

/* DownloadError {{{ */

class DownloadError : public std::runtime_error {
    public:
        DownloadError(const std::string& string)
            : std::runtime_error(string) {}
};

/* }}} */
/* Downloader {{{ */

class Downloader : public QObject
{
    Q_OBJECT

    public:
        Downloader(std::ostream &output) throw (DownloadError);
        virtual ~Downloader() {}

    public:
        void setUrl(const std::string &url) throw (DownloadError);
        std::string getUrl() const;

        void setProgress(ProgressNotifier *notifier);
        void download() throw (DownloadError);

    public slots:
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    private:
        ProgressNotifier        *m_notifier;
        std::string             m_url;
        std::ostream            &m_output;
};

/* }}} */

#endif /* USBPROG_DOWNLOADER_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
