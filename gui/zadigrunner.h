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

/**
 * @file zadigrunner.h
 * @brief Downloads and runs Zadig
 *
 * This file contains the class ZadigRunner.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef ZADIGRUNNER_H
#define ZADIGRUNNER_H

#include <QObject>
#include <QNetworkReply>

#include <usbprog/tempdir.h>

namespace usbprog {
namespace gui {

/**
 * @brief Downloads and runs Zadig
 *
 * Zadig is the libusb installation tool for Windows. This class downloads the program
 * from the internet and executes it. In addition, it generates some configuration files
 * in a temporary directory to make it simpler to use Zadig with USBprog.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class ZadigRunner : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Creates a new ZadigRunner object
     * @param[in] parent the parent widget
     */
    explicit ZadigRunner(QObject *parent = 0);

    /**
     * @brief Desctructor
     *
     * This destructor also deletes the temporary download directory.
     */
    ~ZadigRunner();

public slots:
    /**
     * @brief Starts the download.
     *
     * After starting the download, either the signal downloadError() or the signal
     * downloadFinished() is emitted. During download, the signal downloadProgress()
     * may be emitted repeatedly.
     *
     * Downloading also generates the configuration file in the temporary directory.
     * The temporary directory is deleted in the destructor.
     */
    void startDownload();

    /**
     * @brief Starts the tool
     *
     * You have to call startDownload() before and the signal downloadFinished() must
     * got emitted. The function does not block. The signal terminated() is emitted
     * and inidicates whether the tool finished successfully or with an error. If it was
     * not possible to start the tool, the function returns @c false.
     *
     * @retval @c true if the tool was started in background
     * @retval @c false if it was not possible to start the tool
     */
    bool startTool();

signals:
    /**
     * @brief Indicates an error during download
     * @param[in] text the error message
     */
    void downloadError(const QString &text);

    /**
     * @brief Shows the progress of the download
     * @param[in] progress a value between 0 and 100 %
     */
    void downloadProgress(int progress);

    /**
     * @brief Indicates that the download has finished successfully
     *
     * This includes the generation of the configuration file in the temporary download
     * directory.
     */
    void downloadFinished();

private:
    std::string downloadUrl() const;
    QString filenameInTemp(const QString &name) const;
    bool generateConfigurationFiles();

private slots:
    void downloadProgressSlot(qint64 received, qint64 total);
    void downloadFinishedSlot(QNetworkReply *reply);

private:
    Tempdir m_tempdir;
};


} // end namespace gui
} // end namespace usbprog

#endif // ZADIGRUNNER_H
