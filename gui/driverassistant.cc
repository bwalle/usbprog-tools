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
        tr("<html>This wizard installs the device driver needed on Windows systems to access the "
           "USBprog hardware.<p>"
           "We use <a href=\"http://libusb-win32.sf.net\">libusb-win32</a> as device driver and "
           "<a href=\"http://zadig.akeo.ie\">Zadig</a> as driver installer. That installer solves "
           "the problem that 64 bit versions of Windows enforce digitally signed drivers. <p>"
           "The first step of this wizard is to download the program and to generate the configuration files "
           "for it, the second step is to run Zadig.<p>"
           "You can repeat that steps at any time by running <i><u>D</u>evice</i> &rarr; "
           "<i><u>I</u>nstall driver</i>.</html>")
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
    QWidget *filler1 = new QWidget;
    QWidget *filler2 = new QWidget;

    layout->addWidget(filler1);
    layout->addLayout(buttonLayout);
    layout->addWidget(filler2);
    layout->addWidget(m_downloadProgress);
    layout->addWidget(m_infoLabel);
    layout->setStretchFactor(filler1, 1);
    layout->setStretchFactor(filler2, 8);

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
    m_zadigRunner(zadigRunner),
    m_toolStarted(false)
{
    setTitle(tr("Driver Installation"));

    QLabel *label = new QLabel(
        tr("As mentioned in the introduction, the actual driver installation is performed by <i>Zadig</i>, "
           "an external tool. To install the driver, perform following steps:<p>"
           "<ol>"
           " <li>If not already done, plug USBprog to your computer. Wait until Windows "
           "   is finished with automatic driver installation, if it tries to do so.</li>"
           " <li>Click on the <i>Start Zadig</i> button below. Probably Windows wants you to confirm "
           "   that the application needs to make changes to this computer - the so-called \"User Account "
           "   Control\" (UAC) dialog. Allow this to install the driver.</li>"
           " <li>In Zadig, open the <i><u>D</u>evice</i> menu and choose <i>Load Preset Device</i>.</li>"
           " <li>A file dialog pops up. Choose <tt>usbprog.cfg</tt> and click on <i>Open</i>.</li>"
           ""
           " <li>Now perform the driver installation by clicking on <i>Install driver</i>. This takes some time. "
           " If the driver installation finishes successfully, then Zadig quits automatically. You can now finish "
           " that assistant by clicking on <i>Finish</i>.</li>"
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
    return m_toolStarted;
}

void RunPage::startTool()
{
    m_toolStarted = m_zadigRunner->startTool();
    emit completeChanged();
}

//
// DriverAssistant
//

DriverAssistant::DriverAssistant(QWidget *parent) :
    QWizard(parent),
    m_zadigRunner(new ZadigRunner(this))
{
    setWindowTitle(tr("Driver installation assistant"));

    // show the cancel button even on Mac OS
    // this is just for testing since in the real world, the driver wizard is only
    // used on Windows
    WizardOptions currentOptions = options();
    currentOptions &= ~NoCancelButton;
#if QT_VERSION >= 0x050000
     currentOptions |= NoCancelButtonOnLastPage;
#endif
    setOptions(currentOptions);

    addPage(new IntroPage);
    addPage(new DownloadPage(m_zadigRunner));
    addPage(new RunPage(m_zadigRunner));
}

} // namespace gui
} // namespace usbprog
