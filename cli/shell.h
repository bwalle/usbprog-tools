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
 * @file shell.h
 * @brief Implementation of a simple shell
 *
 * This file contains the complete implementation of a shell.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */

#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdexcept>
#include <usbprog/usbprog.h>

#include <libbw/completion.h>

#include "usbprog.h"

namespace usbprog {
namespace cli {

/* Typedefs {{{ */

class Command;
class CommandArg;

/// Maps strings (command names) to Command objects.
typedef std::map<std::string, Command *> StringCommandMap;

/// List of CommandArg pointers.
typedef std::vector<CommandArg *> CommandArgVector;


/* }}} */
/* CommandArg {{{ */

/**
 * @class CommandArg cli/shell.h
 * @brief Argument for a command
 *
 * Represents a command argument for the Command.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class CommandArg {
    public:
        /**
         * @brief Argument type
         */
        enum Type {
            INVALID,            ///< invalid argument
            INTEGER,            ///< signed number
            UINTEGER,           ///< unsigned number
            FLOAT,              ///< floating point number
            STRING              ///< string
        };

    public:
        /**
         * @brief Returns type type of the argument
         *
         * @return the argument type
         */
        Type getType() const;

        /**
         * @brief Returns the argument as string
         *
         * This function can only be used if getType() returns CommandArg::STRING.
         *
         * @return the argument as string
         * @exception std::runtime_error if the argument type is not CommandArg::STRING
         */
        std::string getString() const
        throw (std::runtime_error);

        /**
         * @brief Returns the argument as signed number
         *
         * This function can only be used if getType() returns CommandArg::INTEGER.
         *
         * @return the argument as number
         * @exception std::runtime_error if the argument type is not CommandArg::INTEGER.
         */
        long long getInteger() const
        throw (std::runtime_error);

        /**
         * @brief Returns the argument as unsigned number
         *
         * This function can only be used if getType() returns CommandArg::UINTEGER.
         *
         * @return the argument as unsigned number
         * @exception std::runtime_error if the argument type is not CommandArg::UINTEGER.
         */
        unsigned long long getUInteger() const
        throw (std::runtime_error);

        /**
         * @brief Returns the argument as floating-point number
         *
         * This function can only be used if getType() returns CommandArg::FLOAT.
         *
         * @return the argument as floating-point number
         * @exception std::runtime_error if the argument type is not CommandArg::FLOAT.
         */
        double getFloat() const
        throw (std::runtime_error);

        /**
         * @brief Sets the argument as string
         *
         * Sets also the argument type to CommandArg::STRING.
         *
         * @param[in] str the string argument
         */
        void setString(const std::string &str);

        /**
         * @brief Sets the argument as integer
         *
         * Sets also the argument type to CommandArg::INTEGER.
         *
         * @param[in] integer the integer argument
         */
        void setInteger(long long integer);

        /**
         * @brief Sets the argument as unsigned integer
         *
         * Sets also the argument type to CommandArg::UINTEGER.
         *
         * @param[in] integer the unsigned integer argument
         */
        void setUInteger(unsigned long long integer);

        /**
         * @brief Sets the argument as floating-point number
         *
         * Sets also the argument type to CommandArg::FLOAT.
         *
         * @param[in] value the floating-point value
         */
        void setFloat(double value);

    public:
        /**
         * @brief Parses a string argument
         *
         * Parses the argument @p str which should be of type @p type.
         *
         * @param[in] str the string argument that should be parsed
         * @param[in] type the type that @p str is assumed to be
         * @return a newly allocated CommandArg object that must be freed by the caller.
         *         The function never returns NULL (if it would be out of memory, the standard
         *         C++ library throws an exception).
         */
        static CommandArg *fromString(const std::string &str, Type type);

    private:
        union {
            unsigned long long ull;
            long long ll;
            double d;
        } m_int;
        std::string m_string;
        Type m_type;
};

/* }}} */
/* Interface for commands {{{ */

/**
 * @class Command cli/shell.h
 * @brief Interface for commands
 *
 * The @p Command is a central interface in the system. Each command in the shell is represented by
 * a class that implements the Command interface. The AbstractCommand provides some help to
 * implement the command.
 *
 * A command supports both options and arguments. The (positional) arguments have a fixed number and
 * must always be provided by the user. The options are (as their name says) optional and start with
 * a dash (<tt>-</tt>). The options are flags only and cannot have arguments.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class Command {
    public:
        virtual ~Command() {}

    public:
        /**
         * @brief Executes the command
         *
         * @param[in] args the arguments for the command
         * @param[in] options a string vector of options for the argument that have been provided by
         *            the user
         * @param[in,out] os the output stream that should be used as output for the command
         * @return @c false if the program should terminate without error (i.e. it was some "exit"
         *         command), @c true if the program should continue as normal.
         * @exception ApplicationError if some error occurred
         */
        virtual bool execute(CommandArgVector   args,
                             core::StringVector options,
                             std::ostream       &os)
        throw (core::ApplicationError) = 0;

        /**
         * @brief Returns the number of arguments the command takes
         *
         * The default implementation AbstractCommand::getArgNumber() returns 0.
         *
         * @return the number of arguments
         */
        virtual size_t getArgNumber() const = 0;

        /**
         * @brief Returns the argument type for the argument @p pos
         *
         * The default implementation AbstractCommand::getArgType() returns always
         * CommandArg::INVALID, regardless of the @p pos.
         *
         * @param[in] pos the number of the argument which must be between 0 (inclusive) and
         *            getArgNumber() (exclusive)
         * @return the command argument type. If @p pos is out of range, CommandArg::INVALID must
         *         be returned.
         */
        virtual CommandArg::Type getArgType(size_t pos) const = 0;

        /**
         * @brief Returns the argument title for the argument @p pos
         *
         * The default implementation AbstractCommand::getArgTitle() returns always the empty string
         * (<tt>""</tt>).
         *
         * @param[in] pos the number of the argument which must be between 0 (inclusive) and
         *            getArgNumber() (exclusive)
         * @return the command argument type. If @p pos is out of range, CommandArg::INVALID must be
         *         returned.
         */
        virtual std::string getArgTitle(size_t pos) const = 0;

        /**
         * @brief Returns the list of supported options
         *
         * Don't forget to prefix the options with the dash (<tt>"-"</tt>). A valid option could be
         * <tt>"nostart"</tt>.
         *
         * The default implementation AbstractCommand::getSupportedOptions() always returns an empty
         * list.
         *
         * @return the option list as described above.
         */
        virtual core::StringVector getSupportedOptions() const = 0;

        /**
         * @brief Returns the name of the command
         *
         * The default implementation in AbstractCommand returns the name string passed in the
         * constructor.
         *
         * @return the name (which is not permitted to be empty)
         */
        virtual std::string name() const = 0;

        /**
         * @brief Returns the alias names of the command
         *
         * A command cannot have only one name but also more names which are called aliases.
         * The default implementation always returns an empty string vector here.
         *
         * @return the alias vector
         */
        virtual core::StringVector aliases() const = 0;

        /**
         * @brief Returns a one line help string
         *
         * @return a string that should be shorter than 50 characters
         * @see printLongHelp()
         */
        virtual std::string help() const = 0;

        /**
         * @brief Prints the help to the given stream
         *
         * This help should be much longer and contain all information that is needed for the user
         * to run the command. Example:
         *
         * @code
         * os << "Name:            exit" << std::endl;
         * os << "Aliases:         quit, q, x" << std::endl;
         * os << std::endl;
         * os << "Description:" << std::endl;
         * os << "Exits the program." << std::endl;
         * @endcode
         *
         * @param[in,out] os the stream where the help should be written to
         */
        virtual void printLongHelp(std::ostream &os) const = 0;

        /**
         * @brief Gets the possible completions
         *
         * @param[in] start the string that should be completed
         * @param[in] pos the number of the argument that should be completed
         * @param[in] option @c true if an option of the command should be completed
         * @param[out] filecompletion set to @c true if readline should actually do a file
         *             completion at the current position
         * @return the list of possible completions which can also be an empty list
         */
        virtual std::vector<std::string> getCompletions(const std::string &start,
                size_t pos, bool option, bool *filecompletion) const = 0;
};

/* }}} */
/* AbstractCommand {{{ */

/**
 * @class AbstractCommand cli/shell.h
 * @brief Abstract implementation for the Command interface
 *
 * This implementation acts as base class for the real implementations in the application and
 * provides some defaults to save typing.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class AbstractCommand : public Command {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new AbstractCommand object.
         *
         * @param[in] name the name of the command that is returned in the name() method.
         */
        AbstractCommand(const std::string &name);

    public:
        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::name()
        std::string name() const;

        /// @copydoc Command::aliases()
        core::StringVector aliases() const;

        /// @copydoc Command::getSupportedOptions()
        core::StringVector getSupportedOptions() const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string &start,
                                                size_t            pos,
                                                bool              option,
                                                bool              *filecompletion) const;

    private:
        std::string m_name;
};

/* }}} */
/* The shell itself {{{ */

/**
 * @class Shell cli/shell.h
 * @brief Command shell
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class Shell : public bw::Completor {
    friend class HelpCommand;
    friend class HelpCmdCommand;

    public:
        /**
         * @brief Constructor
         *
         * Creates a new shell with the given prompt.
         *
         * @param[in] prompt the default prompt of the shell
         */
        Shell(const std::string &prompt);

        /// @brief Destructor
        virtual ~Shell();

    public:
        /**
         * @brief Adds the command @p cmd to the list of commands
         *
         * @param[in] cmd the command to add. After adding the command, it is owned by the shell and
         *            freed in the destructor.
         */
        void addCommand(Command *cmd);

        /**
         * @brief Runs the application (interactive mode)
         *
         * The function doesn't throw.
         */
        void run();

        /**
         * @brief Runs the application (non-interactive mode)
         *
         * Passes @p input to the shell.
         *
         * @param[in] input the user input
         * @param[in] multiple @c true if multiple commands are provided, @c false if only one
         *            command is provided (the second one is only used to call that function from
         *            the interactive run() variant)
         * @return @c true if the shell should be continued to run, @c false otherwise.
         * @throw core::ApplicationError on any error
         */
        bool run(core::StringVector input, bool multiple = true)
        throw (core::ApplicationError);

        /**
         * @brief Complete function
         *
         * This function must supply the completions that are available for
         * the specified text..
         *
         * @param[in] text the currently entered text
         * @param[in] full_text the full line
         * @param[in] start_idx the start index
         * @param[in] end_idx the end index
         * @return the completed values
         */
        std::vector<std::string> complete(const std::string     &text,
                                    const std::string     &full_text,
                                    size_t                start_idx,
                                    ssize_t               end_idx);

    private:
        StringCommandMap m_commands;
        bw::LineReader *m_lineReader;
};

/* }}} */
/* Exit command {{{ */

/**
 * @class ExitCommand cli/shell.h
 * @brief Implements the <tt>"exit"</tt> command.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class ExitCommand : public AbstractCommand {
    public:
        /// @brief Default constructor
        ExitCommand();

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os)
        throw (core::ApplicationError);

        core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;
};

/* }}} */
/* Help command {{{ */

/**
 * @class HelpCommand cli/shell.h
 * @brief Implements the <tt>"help"</tt> command.
 *
 * This command is somewhat special because its implementation iterates through all other commands
 * to display the help. Therefore, the constructor needs to know the shell.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class HelpCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] sh the shell (which is still owned by the caller but the pointer needs to be
         * valid during the whole life time of the HelpCommand object).
         */
        HelpCommand(Shell *sh);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os)
        throw (core::ApplicationError);

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

    private:
        Shell *m_sh;
};

/* }}} */
/* HelpCmd command {{{ */

/**
 * @class HelpCmdCommand cli/shell.h
 * @brief Implements the <tt>"helpcmd"</tt> command.
 *
 * This command is (like the Help command) somewhat special because its implementation iterates
 * through all other commands to display the help. Therefore, the constructor needs to know the
 * shell.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class HelpCmdCommand : public AbstractCommand {
    public:
        /**
         * @brief Constructor
         *
         * @param[in] sh the shell (which is still owned by the caller but the pointer needs to be
         * valid during the whole life time of the HelpCmdCommand object).
         */
        HelpCmdCommand(Shell *sh);

    public:
        /// @copydoc Command::execute()
        bool execute(CommandArgVector   args,
                     core::StringVector options,
                     std::ostream       &os)
        throw (core::ApplicationError);

        /// @copydoc Command::getArgNumber()
        size_t getArgNumber() const;

        /// @copydoc Command::getArgType()
        CommandArg::Type getArgType(size_t pos) const;

        /// @copydoc Command::getArgTitle()
        std::string getArgTitle(size_t pos) const;

        /// @copydoc Command::aliases()
        virtual core::StringVector aliases() const;

        /// @copydoc Command::help()
        std::string help() const;

        /// @copydoc Command::printLongHelp()
        void printLongHelp(std::ostream &os) const;

        /// @copydoc Command::getCompletions()
        std::vector<std::string> getCompletions(const std::string &start,
                size_t pos, bool option, bool *filecompletion) const;

    private:
        Shell *m_sh;
};

/* }}} */

} // end namespace cli
} // end namespace usbprog

#endif /* SHELL_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
