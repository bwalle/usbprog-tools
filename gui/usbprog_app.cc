/*
 * (c) 2011-2014, Bernhard Walle <bernhard@bwalle.de>
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
#include <cstdlib>
#include <cstdio>

#include <usbprog-core/debug.h>
#include <usbprog-core/util.h>

#include <libbw/optionparser.h>

#include <QFileInfo>

#include "usbprog_app.h"
#include "guiconfiguration.h"
#include "config.h"

namespace usbprog {
namespace gui {

namespace {

#if QT_VERSION >= 0x050000
    inline QString qtBasename(const QString &file)
    {
        return QFileInfo(file).fileName();
    }

    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            USBPROG_DEBUG_DBG("[%s:%u] %s", qPrintable(qtBasename(context.file)), context.line, localMsg.constData());
            break;
        case QtWarningMsg:
            USBPROG_DEBUG_INFO("[%s:%u] %s", qPrintable(qtBasename(context.file)), context.line, localMsg.constData());
            break;
        case QtCriticalMsg:
            USBPROG_DEBUG_INFO("[%s:%u] CRITICAL %s", qPrintable(qtBasename(context.file)), context.line, localMsg.constData());
            break;
        case QtFatalMsg:
            USBPROG_DEBUG_INFO("[%s:%u] FATAL %s)", qPrintable(qtBasename(context.file)), context.line, localMsg.constData());
            abort();
        }
    }
#else
    void myMessageOutput(QtMsgType type, const char *msg)
    {
        switch (type) {
        case QtDebugMsg:
            USBPROG_DEBUG_DBG("%s", msg);
            break;
        case QtWarningMsg:
            USBPROG_DEBUG_INFO("%s", msg);
            break;
        case QtCriticalMsg:
            USBPROG_DEBUG_INFO("CRITICAL: %s", msg);
            break;
        case QtFatalMsg:
            USBPROG_DEBUG_INFO(stderr, "FATAL: %s", msg);
            abort();
        }
    }
#endif


} // anonymous namespace

/* UsbprogApplication {{{ */

const QString UsbprogApplication::NAME = "USBprog";
const QString UsbprogApplication::USBPROG_WEBSITE = "http://www.usbprog.org";

UsbprogApplication::UsbprogApplication(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_mainWindow(NULL)
    , m_logfile(NULL)
{
    installQtMessageHandlers();
    initConfig();
}

void UsbprogApplication::initConfig()
{
    GuiConfiguration &conf = GuiConfiguration::config();

    std::string configDir = core::Fileutil::configDir("usbprog");
    if (configDir.size() == 0)
        throw core::ApplicationError("Could not determine configuration directory.");

    conf.setDataDir(configDir);
    conf.setIndexUrl(DEFAULT_INDEX_URL);
}

void UsbprogApplication::installQtMessageHandlers()
{
#if QT_VERSION >= 0x050000
    qInstallMessageHandler(myMessageOutput);
#else
    qInstallMsgHandler(myMessageOutput);
#endif
}

bool UsbprogApplication::parseCommandLine(int argc, char **argv, int &exitCode)
{
    GuiConfiguration &conf = GuiConfiguration::config();

    bw::OptionParser op;
    op.addOption("debug",   'D', bw::OT_FLAG,
                 "Enables debugging output");
    op.addOption("help",    'h', bw::OT_FLAG,
                 "Prints a help message");
    op.addOption("datadir", 'd', bw::OT_STRING,
                 "Uses the specified data " "directory instead of " + conf.getDataDir());
    op.addOption("offline", 'o', bw::OT_FLAG,
                 "Use only the local cache and don't connect to the internet");
    op.addOption("debug",   'D', bw::OT_FLAG,
                 "Enables debug output");

    if (!op.parse(argc, argv))
        throw core::ApplicationError("Parsing command line failed.");

    if (op.getValue("debug").getFlag()) {
        conf.setDebug(true);
        core::Debug::debug()->setLevel(core::Debug::DL_TRACE);
    }

    if (op.getValue("version").getFlag()) {
        std::cerr << "usbprog " << USBPROG_VERSION_STRING << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if (op.getValue("datadir").getType() != bw::OT_INVALID)
        conf.setDataDir(op.getValue("datadir").getString());
    if (op.getValue("offline").getFlag())
        conf.setOffline(true);

    if (conf.getDebug())
        conf.dumpConfig(std::cerr);

    if (conf.isOffline())
        std::cout << "WARNING: You're using usbprog in offline mode!" << std::endl;

    return true;
}

void UsbprogApplication::createAndSetMainWidget()
{
    m_mainWindow = new UsbprogMainWindow(*this);
    m_mainWindow->show();
}

void UsbprogApplication::setDebugLoggingEnabled(bool enabled, const char *filename)
{
    core::Debug *debug = core::Debug::debug();

    debug->setLevel(enabled ? core::Debug::DL_TRACE : core::Debug::DL_INFO);
    if (enabled) {
        m_logfile = fopen(filename, "a+");
        if (!m_logfile)
            throw core::ApplicationError("Unable to open logfile '" + std::string(filename) + "'");
        debug->setFileHandle(m_logfile);
    } else {
        if (m_logfile) {
            debug->setFileHandle(NULL);
            fclose(m_logfile);
            m_logfile = NULL;
        }
    }
}


/* }}} */

} // end namespace gui
} // end namespace usbprog


// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
