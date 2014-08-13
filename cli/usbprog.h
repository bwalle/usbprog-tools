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
 * @file cli/usbprog.h
 * @brief The main application classes
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
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

/// Used when the width of the terminal cannot be retrieved using the ioctl() call.
#define DEFAULT_TERMINAL_WIDTH  80

/* }}} */
/* HashNotifier {{{ */

/**
 * @class HashNotifier cli/usbprog.h
 * @brief ProgressNotifier implementation that displays hashes on the console
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class HashNotifier : public core::ProgressNotifier {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new HashNotifier object with the given terminal width.
         *
         * @param[in] width the width of the terminal (i.e. the width of the progress bar).
         */
        HashNotifier(int width);

        /**
         * @brief Destructor
         */
        ~HashNotifier();

    public:
        /// @copydoc core::ProgressNotifier::progressed()
        int progressed(double total, double now);

        /// @copydoc core::ProgressNotifier::finished()
        void finished();

    private:
        int m_width;
        int m_lastProgress;
};

/* }}} */
/* Usbprog {{{ */

/**
 * @class Usbprog cli/usbprog.h
 * @brief Main class
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class Usbprog {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new Usbprog instance. The arguments are the command line parameters passed from
         * main().
         *
         * @param[in] argc the argument count
         * @param[in] argv the argument vector
         */
        Usbprog(int argc, char *argv[]);

        /**
         * @brief Virtual Destructor
         */
        virtual ~Usbprog();

    public:
        /**
         * @brief Reads the configuration file
         *
         * @exception core::ApplicationError if initializing of the configuration failed
         */
        void initConfig();

        /**
         * @brief Parses the command line given in the constructor
         *
         * @exception core::ApplicationError if initializing of the configuration failed
         */
        void parseCommandLine();

        /**
         * @brief Initializes the firmware pool
         *
         * @exception core::ApplicationError if initializing of the firmware pool failed
         */
        void initFirmwarePool();

        /**
         * @brief Initializes the device manager
         *
         * @exception core::ApplicationError if initializing of the device manager failed
         */
        void initDeviceManager();

        /**
         * @brief Executes the application
         *
         * This function blocks.
         *
         * @exception core::ApplicationError if something went wrong
         */
        void exec();

    protected:
        /**
         * @brief Prints the help
         */
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
