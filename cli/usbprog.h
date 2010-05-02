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
#ifndef USBPROG_H
#define USBPROG_H

#include <stdexcept>

#include <QCoreApplication>

#include <usbprog-core/devices.h>
#include <usbprog/firmwarepool.h>

namespace usbprog {
namespace cli {

/* constants {{{ */

#define DEFAULT_TERMINAL_WIDTH  80

/* }}} */
/* HashNotifier {{{ */

class HashNotifier : public core::ProgressNotifier {
    public:
        HashNotifier(int width);
        ~HashNotifier();

    public:
        int progressed(double total, double now);
        void finished();

    private:
        int m_width;
        int m_lastProgress;
};

/* }}} */
/* Usbprog {{{ */

class Usbprog {
    public:
        Usbprog(int argc, char *argv[]);
        virtual ~Usbprog();

    public:
        void initConfig()
            throw (core::ApplicationError);
        void parseCommandLine();
        void initFirmwarePool()
            throw (core::ApplicationError);
        void initDeviceManager()
            throw (core::ApplicationError);
        void exec()
            throw (core::ApplicationError);

    protected:
        void printHelp();

    private:
        QCoreApplication m_coreApp;
        Firmwarepool *m_firmwarepool;
        std::vector<std::string> m_args;
        core::DeviceManager *m_devicemanager;
        core::ProgressNotifier *m_progressNotifier;
        int m_argc;
        char **m_argv;
};

/* }}} */

} // end namespace cli
} // end namespace usbprog


#endif /* USBPROG_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
