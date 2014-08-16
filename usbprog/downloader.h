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

/**
 * @file downloader.h
 * @brief Download files from the web
 *
 * This file contains the Downloader class and DownloadError.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */

#ifndef USBPROG_DOWNLOADER_H
#define USBPROG_DOWNLOADER_H

#include <stdexcept>
#include <ostream>

#include <QObject>

#include <usbprog-core/progressnotifier.h>
#include <usbprog/usbprog.h>

namespace usbprog {

/* DownloadError {{{ */

/**
 * @class DownloadError usbprog/downloader.h
 * @brief Exception for download errors
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class DownloadError : public std::runtime_error {
public:
    /**
     * @brief Constructor
     *
     * Creates a new DownloadError instance.
     *
     * @param[in] string the error as string
     */
    DownloadError(const std::string& string)
        : std::runtime_error(string) {}
};

/* }}} */
/* Downloader {{{ */

/**
 * @class Downloader usbprog/downloader.h
 * @brief HTTP downloader
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup usbprog
 */
class Downloader : public QObject
{
Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * Creates a new Downloader object.
     *
     * @param[in] output the output stream where the result will be stored.
     * @exception DownloadError
     */
    Downloader(std::ostream &output);

    /**
     * @brief Destructor
     */
    virtual ~Downloader() {}

public:
    /**
     * @brief Sets the URL of the file that should be downloaded.
     *
     * @param[in] url the URL
     */
    void setUrl(const std::string &url);

    /**
     * @brief Returns the URL of the file that should be downloaded.
     *
     * @return the URL
     */
    std::string getUrl() const;

    /**
     * @brief Sets the progress notifier that gets notified during download()
     *
     * @param[in] notifier a pointer to the ProgressNotifier object. The object must be
     *            valid during the whole life time of the Downloader object -- at least
     *            until download() is running. It can be reset with @c NULL. It must be freed by
     *            the caller.
     */
    void setProgress(core::ProgressNotifier *notifier);

    /**
     * @brief Performs the download operation
     *
     * @exception DownloadError if downloading the file failed
     */
    void download();

public slots:

    /**
     * @brief Progress function
     *
     * That's a slot that gets called by Qt during the download operation. It calls the progress
     * notifier that has been registered with setProgress().
     *
     * @param[in] bytesReceived the number of bytes that have been received so far
     * @param[in] bytesTotal the total size of the file
     */
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    /**
     * @brief Finish function
     *
     * That's a slot that gets called by Qt when the downloading has been finished. Since we
     * implement downloading with a "busy loop" (which is not really busy since it is blocking)
     * in the download() function, that function also has to signal the end in the loop.
     */
    void downloadFinished();

private:
    core::ProgressNotifier  *m_notifier;
    std::string             m_url;
    std::ostream            &m_output;
    bool                    m_finished;
};

/* }}} */

} // end namespace usbprog

#endif /* USBPROG_DOWNLOADER_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
