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
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <libbw/completion.h>

#include <usbprog-core/stringutil.h>
#include <usbprog/usbprog.h>

#include "shell.h"
#include "cliconfiguration.h"

namespace usbprog {
namespace cli {

/* AbstractCommand {{{ */

/* -------------------------------------------------------------------------- */
AbstractCommand::AbstractCommand(const std::string &name)
    : m_name(name)
{}

/* -------------------------------------------------------------------------- */
size_t AbstractCommand::getArgNumber() const
{
    return 0;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type AbstractCommand::getArgType(size_t pos) const
{
    return CommandArg::INVALID;
}

/* -------------------------------------------------------------------------- */
std::string AbstractCommand::getArgTitle(size_t pos) const
{
    return "";
}

/* -------------------------------------------------------------------------- */
std::string AbstractCommand::name() const
{
    return m_name;
}

/* -------------------------------------------------------------------------- */
core::StringVector AbstractCommand::aliases() const
{
    return core::StringVector();
}

/* -------------------------------------------------------------------------- */
core::StringVector AbstractCommand::getSupportedOptions() const
{
    return core::StringVector();
}

/* -------------------------------------------------------------------------- */
core::StringVector AbstractCommand::getCompletions(const std::string &start,
                                                   size_t            pos,
                                                   bool              option,
                                                   bool              *filecompletion) const
{
    return core::empty_element_sv();
}

/* }}} */
/* CommandArg {{{ */

/* -------------------------------------------------------------------------- */
CommandArg::Type CommandArg::getType() const
{
    return m_type;
}

/* -------------------------------------------------------------------------- */
std::string CommandArg::getString() const
    throw (std::runtime_error)
{
    if (m_type != STRING)
        throw std::runtime_error("Not a string");

    return m_string;
}

/* -------------------------------------------------------------------------- */
long long CommandArg::getInteger() const
    throw (std::runtime_error)
{
    if (m_type != INTEGER)
        throw std::runtime_error("Not an integer");

    return m_int.ll;
}

/* -------------------------------------------------------------------------- */
unsigned long long CommandArg::getUInteger() const
    throw (std::runtime_error)
{
    if (m_type != UINTEGER)
        throw std::runtime_error("Not an unsigned integer");

    return m_int.ull;
}

/* -------------------------------------------------------------------------- */
double CommandArg::getFloat() const
    throw (std::runtime_error)
{
    if (m_type != FLOAT)
        throw std::runtime_error("Not a float");

    return m_int.d;
}

/* -------------------------------------------------------------------------- */
void CommandArg::setString(const std::string &string)
{
    m_type = STRING;
    m_string = string;
}

/* -------------------------------------------------------------------------- */
void CommandArg::setInteger(long long integer)
{
    m_type = INTEGER;
    m_int.ll = integer;
}

/* -------------------------------------------------------------------------- */
void CommandArg::setUInteger(unsigned long long integer)
{
    m_type = UINTEGER;
    m_int.ull = integer;
}

/* -------------------------------------------------------------------------- */
void CommandArg::setFloat(double value)
{
    m_type = FLOAT;
    m_int.d = value;
}

/* -------------------------------------------------------------------------- */
CommandArg *CommandArg::fromString(const std::string &str, CommandArg::Type type)
{
    std::stringstream string;
    CommandArg *ret = new CommandArg;

    string << str;

    switch (type) {
        case STRING:
            ret->setString(string.str());
            break;

        case INTEGER: {
                long long val;
                string >> val;
                ret->setInteger(val);
                break;
          }

        case UINTEGER: {
                unsigned long long val;
                string >> val;
                ret->setUInteger(val);
                break;
            }

        case FLOAT: {
                double val;
                string >> val;
                ret->setFloat(val);
                break;
            }

        case INVALID:
            break;
    }

    return ret;
}

/* }}} */
/* Shell {{{ */

/* -------------------------------------------------------------------------- */
Shell::Shell(const std::string &prompt)
{
    m_lineReader = bw::LineReader::defaultLineReader(prompt);
    try {
        m_lineReader->readHistory(CliConfiguration::config().getHistoryFile());
    } catch (const core::IOError &)
    {}

    if (m_lineReader->haveCompletion())
        m_lineReader->setCompletor(this);

    addCommand(new ExitCommand);
    addCommand(new HelpCommand(this));
    addCommand(new HelpCmdCommand(this));
}

/* -------------------------------------------------------------------------- */
Shell::~Shell()
{
    std::vector<Command *> deleteList;

    for (StringCommandMap::const_iterator it = m_commands.begin();
            it != m_commands.end(); ++it)
        if (it->second->name() == it->first)
            deleteList.push_back(it->second);

    for (std::vector<Command *>::const_iterator it = deleteList.begin();
            it != deleteList.end(); ++it)
        delete *it;

    try {
        m_lineReader->writeHistory(CliConfiguration::config().getHistoryFile());
    } catch (const core::IOError &ioe) {
        std::cerr << "Error when saving history: " << ioe.what() << std::endl;
    }
    delete m_lineReader;
}

/* -------------------------------------------------------------------------- */
void Shell::addCommand(Command *cmd)
{
    // if it's already in the map, remove it first and free the memory
    StringCommandMap::iterator it = m_commands.find(cmd->name());
    if (it != m_commands.end()) {
        delete it->second;
        m_commands.erase(it);
    }

    m_commands[cmd->name()] = cmd;

    core::StringVector aliases = cmd->aliases();
    for (core::StringVector::const_iterator it = aliases.begin();
            it != aliases.end(); ++it)
        m_commands[*it] = cmd;
}

/* -------------------------------------------------------------------------- */
std::vector<std::string> Shell::complete(const std::string     &text,
                                         const std::string     &full_text,
                                         size_t                start_idx,
                                         int                   end_idx)
{
    //
    // command completion
    //
    if (start_idx == 0) {
        core::StringVector result;
        for (StringCommandMap::const_iterator it = m_commands.begin();
             it != m_commands.end(); ++it) {
            std::string cmd = it->first;
            if (core::str_starts_with(cmd, text))
                result.push_back(cmd);
        }
        return result;
    }

    //
    // argument completion
    //
    core::ShellStringTokenizer tok(full_text);
    core::StringVector vec = tok.tokenize();

    if (vec.size() <= 0)
        return core::empty_element_sv();

    // get command for now
    Command *cmd = m_commands[vec[0]];
    if (!cmd)
        return core::empty_element_sv();

    // options
    if (text.size() > 0 && text[0] == '-') {
        if (cmd->getSupportedOptions().size() == 0)
            return core::empty_element_sv();
        else
            return cmd->getCompletions(text, 0, true, NULL);
    } else {
        size_t pos = vec.size() - 1;
        if (text.size() > 0)
            pos--;
        for (core::StringVector::const_iterator it = vec.begin(); it != vec.end(); ++it)
            // don't count a position for an option
            if ((*it).size() > 0 && (*it)[0] == '-')
                pos--;
        if (cmd->getArgNumber() <= pos)
            return core::empty_element_sv();
        else {
            bool filecompletion = false;
            core::StringVector completions = cmd->getCompletions(
                    text, pos, false, &filecompletion);
            if (completions.size() == 0 && !filecompletion)
                return core::empty_element_sv();
            else
                return completions;
        }
    }
}

/* -------------------------------------------------------------------------- */
void Shell::run()
{
    bool result = true;

    while (!m_lineReader->eof() && result) {
        core::ShellStringTokenizer tok(m_lineReader->readLine());
        core::StringVector vec = tok.tokenize();

        if (vec.size() == 0)
            continue;

        try {
            result = run(vec, false);
        } catch (const core::ApplicationError &err) {
            std::cout << err.what() << std::endl;
        }
    }

    // output a newline if the readline() library has encountered
    // an EOF
    if (m_lineReader->eof())
        std::cout << std::endl;
}

/* -------------------------------------------------------------------------- */
bool Shell::run(core::StringVector input, bool multiple)
    throw (core::ApplicationError)
{
    bool result = true;
    int loop = 0;

    if (input.size() == 0)
        throw core::ApplicationError("Input size == 0");

    do {
        std::string cmdstr = input[0];
        std::string execstr = cmdstr;
        input.erase(input.begin());
        StringCommandMap::const_iterator it = m_commands.find(cmdstr);
        if (it == m_commands.end())
            throw core::ApplicationError("Invalid command");
        Command *cmd = it->second;

        // separate options from arguments
        core::StringVector options;
        for (core::StringVector::iterator it = input.begin(); it != input.end(); ++it) {
            std::string option = *it;

            if (option == "--") {
                // treat "--" like with GNU getopt
                input.erase(it);
                break;
            } else if (option[0] != '-') {
                // the first non-option argument ends the possible options
                break;
            } else {
                options.push_back(option);
                execstr += " " + option;

                core::StringVector supported = cmd->getSupportedOptions();
                if (find(supported.begin(), supported.end(), option) == supported.end())
                    throw core::ApplicationError("Option '" + option + "' not supported.");

                input.erase(it);
            }
        }

        // check number of arguments
        if (multiple && cmd->getArgNumber() > input.size())
            throw core::ApplicationError(cmdstr + ": Not enough arguments provided");
        if (!multiple && cmd->getArgNumber() < input.size())
            throw core::ApplicationError(cmdstr + ": Too much arguments provided.");

        CommandArgVector vec;
        for (unsigned int argNo = 0; argNo < cmd->getArgNumber(); argNo++) {
            std::string argstr;
            if (input.size() > 0) {
                argstr = input[0];
                input.erase(input.begin());
            } else {
                std::string prompt = cmd->getArgTitle(argNo) + "> ";
                argstr = m_lineReader->readLine(prompt.c_str());
            }

            execstr += " " + argstr;
            CommandArg *arg = CommandArg::fromString(argstr,
                    cmd->getArgType(argNo));
            vec.push_back(arg);
        }

        try {
            if (multiple && (input.size() > 0 || loop != 0))
                std::cout << "===> " << execstr << std::endl;
            loop++;
            result = cmd->execute(vec, options, std::cout);
            if (multiple && result && input.size() > 0)
                std::cout << std::endl;

        } catch (const core::ApplicationError &ex) {
            std::cout << ex.what() << std::endl;
        }

        // free memory
        for (CommandArgVector::const_iterator it = vec.begin();
                it != vec.end(); ++it)
            delete *it;

    } while (result && input.size() > 0 && multiple);

    return result;
}

/* }}} */
/* ExitCommand {{{ */

/* -------------------------------------------------------------------------- */
ExitCommand::ExitCommand()
    : AbstractCommand("exit")
{}

/* -------------------------------------------------------------------------- */
bool ExitCommand::execute(CommandArgVector   args,
                          core::StringVector options,
                          std::ostream       &os)
    throw (core::ApplicationError)
{
    return false;
}

/* -------------------------------------------------------------------------- */
core::StringVector ExitCommand::aliases() const
{
    core::StringVector sv;
    sv.push_back("quit");
    sv.push_back("q");
    sv.push_back("x");
    return sv;
}

/* -------------------------------------------------------------------------- */
std::string ExitCommand::help() const
{
    return "Exits the program";
}

/* -------------------------------------------------------------------------- */
void ExitCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            exit" << std::endl;
    os << "Aliases:         quit, q, x" << std::endl;
    os << std::endl;
    os << "Description:" << std::endl;
    os << "Exits the program." << std::endl;
}

/* }}} */
/* HelpCommand {{{ */

/* -------------------------------------------------------------------------- */
HelpCommand::HelpCommand(Shell *sh)
    : AbstractCommand("help")
{
    m_sh = sh;
}

/* -------------------------------------------------------------------------- */
bool HelpCommand::execute(CommandArgVector   args,
                          core::StringVector options,
                          std::ostream       &os)
    throw (core::ApplicationError)
{
    for (StringCommandMap::const_iterator it = m_sh->m_commands.begin();
            it != m_sh->m_commands.end(); ++it) {
        if (it->second->name() == it->first)
            os << std::setw(20) << std::left << it->second->name()
                 << it->second->help() << std::endl;
    }

    os << std::endl;
    os << "To get more information about a specific command, use "
       << "\"helpcmd command\"." << std::endl;

    return true;
}

/* -------------------------------------------------------------------------- */
std::string HelpCommand::help() const
{
    return "Prints an overview about all commands.";
}

/* -------------------------------------------------------------------------- */
void HelpCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            help" << std::endl;
    os << "Arguments:       command" << std::endl;
    os << std::endl;
    os << "Description:" << std::endl;
    os << "Prints an overview about all commands. To get help for a specific" << std::endl;
    os << "command, use \"helpcmd command\"." << std::endl;
}

/* }}} */
/* HelpCmdCommand  {{{ */

/* -------------------------------------------------------------------------- */
HelpCmdCommand::HelpCmdCommand(Shell *sh)
    : AbstractCommand("helpcmd")
{
    m_sh = sh;
}

/* -------------------------------------------------------------------------- */
bool HelpCmdCommand::execute(CommandArgVector    args,
                             core::StringVector  options,
                             std::ostream        &os)
    throw (core::ApplicationError)
{
    std::string cmd = args[0]->getString();

    if (m_sh->m_commands.find(cmd) == m_sh->m_commands.end())
        os << "Invalid command: " + cmd << std::endl;
    else {
        Command *c = m_sh->m_commands.find(cmd)->second;
        c->printLongHelp(os);
    }

    return true;
}

/* -------------------------------------------------------------------------- */
size_t HelpCmdCommand::getArgNumber() const
{
    return 1;
}

/* -------------------------------------------------------------------------- */
CommandArg::Type HelpCmdCommand::getArgType(size_t pos) const
{
    switch (pos) {
        case 0:     return CommandArg::STRING;
        default:    return CommandArg::INVALID;
    }
}

/* -------------------------------------------------------------------------- */
std::string HelpCmdCommand::getArgTitle(size_t pos) const
{
    switch (pos) {
        case 0:     return "command";
        default:    return "";
    }
}

/* -------------------------------------------------------------------------- */
core::StringVector HelpCmdCommand::aliases() const
{
    core::StringVector sv;
    sv.push_back("?");
    return sv;
}

/* -------------------------------------------------------------------------- */
std::vector<std::string> HelpCmdCommand::getCompletions(const std::string &start,
                                                        size_t            pos,
                                                        bool              option,
                                                        bool              *filecompletion) const
{
    if (pos != 0)
        return core::StringVector();

    core::StringVector result;
    for (StringCommandMap::const_iterator it = m_sh->m_commands.begin();
            it != m_sh->m_commands.end(); ++it) {
        std::string cmd = it->first;
        if (core::str_starts_with(cmd, start))
            result.push_back(cmd);
    }
    return result;
}


/* -------------------------------------------------------------------------- */
std::string HelpCmdCommand::help() const
{
    return "Prints help for a command";
}

/* -------------------------------------------------------------------------- */
void HelpCmdCommand::printLongHelp(std::ostream &os) const
{
    os << "Name:            helpcmd" << std::endl;
    os << "Aliases:         ?" << std::endl;
    os << "Arguments:       1) the command" << std::endl;
    os << std::endl;
    os << "Description:" << std::endl;
    os << "Prints the help for a given command. To get an overview about" << std::endl;
    os << "all commands, use \"help\"." << std::endl;
}

/* }}} */

} // end namespace cli
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
