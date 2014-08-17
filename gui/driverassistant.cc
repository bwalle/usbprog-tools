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

#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

#include "driverassistant.h"
#include "zadigrunner.h"

namespace usbprog {
namespace gui {

//
// IntroPage
//

IntroPage::IntroPage(QWidget *parent) :
    QWizardPage(parent)
{
    setTitle(tr("Introduction"));

    QLabel *label = new QLabel(
        tr("<html>This wizard installs the device driver needed on Windows systems to access USBprog.<p>"
           "We use <a href=\"http://libusb-win32.sf.net\">libusb-win32</a> as device driver and "
           "<a href=\"http://zadig.akeo.ie\">Zadig</a> as driver installer. This installer solves "
           "the problem with digital signatures also on 64 bit versions of Windows 7 and Windows 8 "
           "which prevent installation of unsigned drivers. <p>"
           "So the first step is to download the program and to generate the configuration files for it, "
           "the second step is to run Zadig.<p>"
           "You can repeat that steps at any time by running <i>Device</i> &rarr; <i>Install driver</i>."
           "</html>")
        );
    label->setWordWrap(true);
    label->setOpenExternalLinks(true);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

//
// DownloadPage
//

DownloadPage::DownloadPage(ZadigRunner *zadigRunner, QWidget *parent) :
    QWizardPage(parent),
    m_downloadComplete(false),
    m_zadigRunner(zadigRunner)
{
    setTitle(tr("Download"));

    QLabel *label = new QLabel(
        tr("Start the download of <i>Zadig</i> by clicking on <i>Start download</i>:")
    );
    label->setWordWrap(true);
    m_downloadProgress = new QProgressBar;
    m_infoLabel = new QLabel;
    m_startButton = new QPushButton(tr("&Start download"));
    connect(m_startButton, SIGNAL(clicked()), SLOT(startDownload()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(new QWidget);
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(new QWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    QWidget *filler = new QWidget;
    layout->addLayout(buttonLayout);
    layout->addWidget(filler);
    layout->addWidget(m_downloadProgress);
    layout->addWidget(m_infoLabel);
    layout->setStretchFactor(filler, 1);

    setLayout(layout);

    connect(m_zadigRunner, SIGNAL(downloadError(QString)), SLOT(showDownloadError(QString)));
    connect(m_zadigRunner, SIGNAL(downloadProgress(int)), m_downloadProgress, SLOT(setValue(int)));
    connect(m_zadigRunner, SIGNAL(downloadFinished()), SLOT(downloadFinished()));
}

bool DownloadPage::isComplete() const
{
    return m_downloadComplete;
}

void DownloadPage::startDownload()
{
    m_infoLabel->setText(tr("Downloading Zadig..."));
    m_zadigRunner->startDownload();
    m_startButton->setEnabled(false);
}

void DownloadPage::showDownloadError(const QString &msg)
{
    m_infoLabel->setText(tr("Error: %1.").arg(msg));
    m_startButton->setEnabled(true);
}

void DownloadPage::downloadFinished()
{
    m_downloadComplete = true;
    m_infoLabel->setText(tr("Download finished!"));
    emit completeChanged();
}

//
// RunPage
//

RunPage::RunPage(ZadigRunner *zadigRunner, QWidget *parent) :
    QWizardPage(parent),
    m_runComplete(false),
    m_zadigRunner(zadigRunner)
{
    setTitle(tr("Driver Installation"));

    QLabel *label = new QLabel(
        tr("As mentioned on the first page, the actual driver installation is performed by <i>Zadig</i>, "
           "an external tool. To install the driver, perform following steps:<p>"
           "<ol>"
           " <li>If not already done, plug USBprog to your computer. Wait until Windows "
           "     finishes is finished with automatic driver installation, if it tries to do so.</li>"
           " <li>Click on the <i>Start Zadig</i> button below. A new window pops up.</li>"
           " <li>Now open .... ?</li>"
           "</ol>")
    );
    label->setWordWrap(true);
    m_infoLabel = new QLabel;
    m_startButton = new QPushButton(tr("&Start Zadig"));
    connect(m_startButton, SIGNAL(clicked()), SLOT(startTool()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(new QWidget);
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addWidget(new QWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    QWidget *filler = new QWidget;
    layout->addLayout(buttonLayout);
    layout->addWidget(filler);
    layout->addWidget(m_infoLabel);
    layout->setStretchFactor(filler, 1);

    setLayout(layout);
}

bool RunPage::isComplete() const
{
    return m_runComplete;
}

void RunPage::startTool()
{
}

//
// DriverAssistant
//

DriverAssistant::DriverAssistant(QWidget *parent) :
    QWizard(parent),
    m_zadigRunner(new ZadigRunner(this))
{
    // show the cancel button even on Mac OS
    // this is just for testing since in the real world, the driver wizard is only
    // used on Windows
    WizardOptions currentOptions = options();
    currentOptions &= ~NoCancelButton;
    setOptions(currentOptions);

    addPage(new IntroPage);
    addPage(new DownloadPage(m_zadigRunner));
    addPage(new RunPage(m_zadigRunner));
}

} // namespace gui
} // namespace usbprog
