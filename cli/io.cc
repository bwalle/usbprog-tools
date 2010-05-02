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
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include "config.h"

#if HAVE_LIBREADLINE
# include <readline/readline.h>
# include <readline/history.h>
#endif

#include <usbprog-core/stringutil.h>
#include <usbprog/usbprog.h>
#include "io.h"

namespace usbprog {
namespace cli {

/* class definitions {{{ */

#if HAVE_LIBREADLINE
class ReadlineLineReader : public AbstractLineReader {
    public:
        ReadlineLineReader(const std::string &prompt);

    public:
        std::string readLine(const char *prompt = NULL);
        void readHistory(const std::string &file)
            throw (core::IOError);
        void writeHistory(const std::string &file)
            throw (core::IOError);
        bool haveHistory() const;

        bool haveCompletion() const;
        void setCompletor(Completor *comp);

    private:
        Completor *m_completor;
};
#endif

class SimpleLineReader : public AbstractLineReader {
    public:
        SimpleLineReader(const std::string &prompt);

    public:
        std::string readLine(const char *prompt = NULL);
};

/* }}} */
/* LineReader {{{ */

/* -------------------------------------------------------------------------- */
LineReader *LineReader::defaultLineReader(const std::string &prompt)
{
#if HAVE_LIBREADLINE
    return new ReadlineLineReader(prompt);
#else
    return new SimpleLineReader(prompt);
#endif
}

/* }}} */
/* AbstractLineReader {{{ */

/* -------------------------------------------------------------------------- */
AbstractLineReader::AbstractLineReader(const std::string &prompt)
    : m_prompt(prompt), m_eof(false)
{}

/* -------------------------------------------------------------------------- */
std::string AbstractLineReader::getPrompt() const
{
    return m_prompt;
}

/* -------------------------------------------------------------------------- */
void AbstractLineReader::setEof(bool eof)
{
    m_eof = eof;
}

/* -------------------------------------------------------------------------- */
bool AbstractLineReader::eof() const
{
    return m_eof;
}

/* -------------------------------------------------------------------------- */
void AbstractLineReader::readHistory(const std::string &file)
    throw (core::IOError)
{}

/* -------------------------------------------------------------------------- */
void AbstractLineReader::writeHistory(const std::string &file)
    throw (core::IOError)
{}

/* -------------------------------------------------------------------------- */
bool AbstractLineReader::haveHistory() const
{
    return false;
}

/* -------------------------------------------------------------------------- */
bool AbstractLineReader::haveCompletion() const
{
    return false;
}

/* -------------------------------------------------------------------------- */
void AbstractLineReader::setCompletor(Completor *comp)
{}

/* }}} */
/* SimpleLineReader {{{ */

/* -------------------------------------------------------------------------- */
SimpleLineReader::SimpleLineReader(const std::string &prompt)
    : AbstractLineReader(prompt)
{}

/* -------------------------------------------------------------------------- */
std::string SimpleLineReader::readLine(const char *prompt)
{
    std::string ret;
    if (!prompt)
        std::cout << getPrompt();
    else
        std::cout << prompt;
    std::getline(std::cin, ret, '\n');
    if (std::cout.eof())
        setEof(true);
    return ret;
}

/* }}} */
/* ReadlineLineReader {{{ */

#if HAVE_LIBREADLINE

/* completion stuff, not really object oriented :-( */

/* -------------------------------------------------------------------------- */
Completor *g_current_completor;

/* -------------------------------------------------------------------------- */
char **readline_line_reader_complete(const char *text, int start, int end)
{
    core::StringVector completions = g_current_completor->complete(
            text, std::string(rl_line_buffer), start, end);

    if (completions.size() == 0)
        return NULL;

    // the first entry is the string which replaces text, so it must be
    // the largest string which is common to each entry in completions
    std::string replacement = completions[0];
    for (core::StringVector::const_iterator it = completions.begin();
            it != completions.end(); ++it) {
        size_t len = std::min(it->size(), replacement.size());
        for (unsigned int mismatch = 0; mismatch < len; ++mismatch) {
            if ((*it)[mismatch] != replacement[mismatch]) {
                replacement = replacement.substr(0, mismatch);
                break;
            }
        }
    }

    return core::stringvector_to_array(completions);
}

/* -------------------------------------------------------------------------- */
ReadlineLineReader::ReadlineLineReader(const std::string &prompt)
    : AbstractLineReader(prompt)
{}

/* -------------------------------------------------------------------------- */
std::string ReadlineLineReader::readLine(const char *prompt)
{
    char *line_read;
    std::string ret;

    line_read = readline(prompt ? prompt : getPrompt().c_str());
    if (!line_read)
        setEof(true);
    else if (*line_read) {
        if (!prompt)
            add_history(line_read);
        ret = std::string(line_read);
        std::free(line_read);
    }

    return ret;
}

/* -------------------------------------------------------------------------- */
void ReadlineLineReader::readHistory(const std::string &file)
    throw (core::IOError)
{
    int ret = read_history(file.c_str());
    if (ret < 0)
        throw core::IOError(std::string("Reading readline history failed: ")
                + std::strerror(errno));
}

/* -------------------------------------------------------------------------- */
void ReadlineLineReader::writeHistory(const std::string &file)
    throw (core::IOError)
{
    int ret = write_history(file.c_str());
    if (ret < 0)
        throw core::IOError(std::string("Writing readline history failed: ")
                + std::strerror(errno));
}

/* -------------------------------------------------------------------------- */
bool ReadlineLineReader::haveHistory() const
{
    return true;
}

/* -------------------------------------------------------------------------- */
bool ReadlineLineReader::haveCompletion() const
{
    return true;
}

/* -------------------------------------------------------------------------- */
void ReadlineLineReader::setCompletor(Completor *comp)
{
    m_completor = comp;
    g_current_completor = m_completor;

    if (m_completor)
        rl_attempted_completion_function = readline_line_reader_complete;
    else
        rl_attempted_completion_function = NULL;
}

/* }}} */

} // end namespace cli
} // end namespace usbprog

#endif

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
