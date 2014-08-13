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
 * @file commands.h
 * @brief User commands
 *
 * This file contains all commands that are specific to the USBprog application in the shell. The
 * generic commands like "help" or "exit" are in the shell.h file.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */


#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <usbprog/firmwarepool.h>
#include <usbprog/usbprog.h>

#include "shell.h"

namespace usbprog {
namespace cli {

/* ListCommand {{{ */

/**
 * @class ListCommand cli/commands.h
 * @brief Implements the <tt>"list"</tt> command.
 *
 * This command lists all firmwares.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class ListCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new ListCommand instance.
         *
         * @param[in] firmwarepool the firmwarepool which is still owned by the caller
         */
        ListCommand(Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        Firmwarepool *m_firmwarepool;
};

/* }}} */
/* InfoCommand {{{ */

/**
 * @class InfoCommand cli/commands.h
 * @brief Implements the <tt>"info"</tt> command.
 *
 * This command prints information for one firmware.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class InfoCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new InfoCommand instance.
         *
         * @param[in] firmwarepool the firmwarepool which is still owned by the caller
         */
        InfoCommand(Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        Firmwarepool *m_firmwarepool;
};

/* }}} */
/* PinCommand {{{ */

/**
 * @class PinCommand cli/commands.h
 * @brief Implements the <tt>"pins"</tt> command.
 *
 * This command prints informations for pins.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class PinCommand : public AbstractCommand {
    public:

        /**
         * @brief Constructor
         *
         * Creates a new PinCommand instance.
         *
         * @param[in] firmwarepool the Firmwarepool which is still owned by the caller
         */
        PinCommand(Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector       args,
                     core::StringVector     options,
                     std::ostream           &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

    private:
        Firmwarepool *m_firmwarepool;
};

/* }}} */
/* DownloadCommand {{{ */

/**
 * @class DownloadCommand cli/commands.h
 * @brief Implements the <tt>"download"</tt> command.
 *
 * This command downloads a firmware and stores it on disk.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class DownloadCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new DownloadCommand.
         *
         * @param[in] firmwarepool the firmware pool
         */
        DownloadCommand(Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector       args,
                     core::StringVector     options,
                     std::ostream           &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

    protected:
        /**
         * @brief Downloads all firmwares
         *
         * @param[in,out] os the stream where the output should be printed to
         * @return always @c true
         * @exception core::IOError whenever downloading fails
         */
        bool downloadAll(std::ostream &os);

    private:
        Firmwarepool *m_firmwarepool;
};

/* }}} */
/* CacheCommand {{{ */

/**
 * @class CacheCommand cli/commands.h
 * @brief Implement the <tt>"cache"</tt> command.
 *
 * Provides a cleanup and a delete action for the firmware cache.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class CacheCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new CacheCommand.
         *
         * @param[in] firmwarepool the firmware pool
         */
        CacheCommand(Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector       args,
                     core::StringVector     options,
                     std::ostream           &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

    private:
        Firmwarepool *m_firmwarepool;
};

/* }}} */
/* DevicesCommand {{{ */

/**
 * @class DevicesCommand cli/commands.h
 * @brief Implements the <tt>"devices"</tt> command.
 *
 * Lists all available devices.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class DevicesCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] deviceManager the device manager (still owned by the caller)
         * @param[in] firmwarepool the firmware pool (still owned by the caller)
         */
        DevicesCommand(core::DeviceManager *deviceManager, Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector       args,
                     core::StringVector     options,
                     std::ostream           &os);

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        core::DeviceManager *m_deviceManager;
        Firmwarepool        *m_firmwarepool;
};

/* }}} */
/* DeviceCommand {{{ */

/**
 * @class DeviceCommand cli/commands.h
 * @brief Implements the <tt>"device"</tt> command.
 *
 * Sets the update device to a specific device.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class DeviceCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of DeviceCommand.
         *
         * @param[in] deviceManager the device manager (that is still owned by the caller)
         * @param[in] firmwarepool the firmware pool (that is still owned by the caller)
         */
        DeviceCommand(core::DeviceManager *deviceManager, Firmwarepool *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

    private:
        core::DeviceManager *m_deviceManager;
        Firmwarepool        *m_firmwarepool;
};

/* }}} */
/* UploadCommand {{{ */

/**
 * @class UploadCommand cli/commands.h
 * @brief Implements the <tt>"upload"</tt> command.
 *
 * Uploads a specific firmware.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class UploadCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of DeviceCommand.
         *
         * @param[in] deviceManager the device manager (that is still owned by the caller)
         * @param[in] firmwarepool the firmware pool (that is still owned by the caller)
         */
        UploadCommand(core::DeviceManager *deviceManager, Firmwarepool  *firmwarepool);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::getSupportedOptions()
        core::StringVector getSupportedOptions() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string   &start,
                                                size_t              pos,
                                                bool                option,
                                                bool                *filecompletion) const;

    private:
        core::DeviceManager *m_deviceManager;
        Firmwarepool        *m_firmwarepool;
};

/* }}} */
/* StartCommand {{{ */

/**
 * @class StartCommand cli/commands.h
 * @brief Implements the <tt>"start"</tt> command
 *
 * Starts the current firmware. That is already done by the <tt>"upload"</tt> command, but with this
 * command it can be done explicitly.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class StartCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of StartCommand.
         *
         * @param[in] deviceManager the device manager which is still owned by the caller
         */
        StartCommand(core::DeviceManager *deviceManager);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        core::DeviceManager *m_deviceManager;
};

/* }}} */
/* ResetCommand {{{ */

/**
 * @class ResetCommand cli/commands.h
 * @brief Implements the <tt>"reset"</tt> command
 *
 * Resets the selected device.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class ResetCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of a ResetCommand object.
         *
         * @param[in] deviceManager the device manager that is still owned by the caller
         */
        ResetCommand(core::DeviceManager *deviceManager);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        core::DeviceManager *m_deviceManager;
};

/* }}} */
/* CopyingCommand {{{ */

/**
 * @class CopyingCommand cli/commands.h
 * @brief Implements the <tt>"copying"</tt> command
 *
 * Just print the license of the application.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class CopyingCommand : public AbstractCommand {
    public:
        CopyingCommand();

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os);

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;
};

/* }}} */

} // end namespace cli
} // end namespace usbprog

#endif /* COMMANDS_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
