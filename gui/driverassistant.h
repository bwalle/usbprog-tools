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
 * @file driverassistant.h
 * @brief Assitant to install the USBprog driver
 *
 * This file contains the class ZadigRunner.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef GUI_DRIVERASSISTANT_H_
#define GUI_DRIVERASSISTANT_H_

#include <QWizard>
#include <QProgressBar>
#include <QLabel>

#include "zadigrunner.h"

namespace usbprog {
namespace gui {

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent=0);
};


class DownloadPage : public QWizardPage
{
    Q_OBJECT

public:
    DownloadPage(ZadigRunner *zadigRunner, QWidget *parent=0);

    virtual bool isComplete() const;

private slots:
    void startDownload();
    void showDownloadError(const QString &msg);
    void downloadFinished();

private:
    QProgressBar *m_downloadProgress;
    QLabel *m_infoLabel;
    ZadigRunner *m_zadigRunner;
    QPushButton *m_startButton;
    bool m_downloadComplete;
};


class RunPage : public QWizardPage
{
    Q_OBJECT

public:
    RunPage(ZadigRunner *zadigRunner, QWidget *parent=0);

    virtual bool isComplete() const;

private slots:
    void startTool();

private:
    QLabel *m_infoLabel;
    ZadigRunner *m_zadigRunner;
    QPushButton *m_startButton;
    bool m_toolStarted;
};



class DriverAssistant : public QWizard
{
    Q_OBJECT

public:
    DriverAssistant(QWidget *parent=0);

private slots:

private:
    QProgressBar *m_downloadProgress;
    ZadigRunner *m_zadigRunner;
};

} // namespace gui
} // namespace usbprog

#endif // GUI_DRIVERASSISTANT_H_
