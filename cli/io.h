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
 * @file io.h
 * @brief Command line completion and readline wrapper
 *
 * This file contains absractions for the readline wrapper and the command line completion based on.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */

#ifndef IO_H
#define IO_H

#include <usbprog-core/error.h>

namespace usbprog {
namespace cli {

/* Interface for completors {{{ */

/**
 * @class Completor cli/io.h
 * @brief Abstration for the completion
 *
 * To have completion support with libreadline, the shell implements the Completor interface.
 * The only method Completor::complete() gets called when the completion mechanism wants to know the
 * possible completions.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class Completor {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~Completor() {}

    public:
        /**
         * @brief Completor function that gets called by the completion
         *
         * @param[in] text the text that should be completed
         * @param[in] full_text the full line that should be completed
         * @param[in] start_idx the start index
         * @param[in] end_idx the end index
         * @return all completions
         */
        virtual core::StringVector complete(const std::string     &text,
                                            const std::string     &full_text,
                                            unsigned int          start_idx,
                                            unsigned int          end_idx) = 0;
};

/* }}} */
/* Interface for a linereader {{{ */

/**
 * @class LineReader cli/io.h
 * @brief Interface for a line reader
 *
 * This is an object oriented approach to wrap libreadline. The idea is that there should be
 * multiple readline implementations that implement all the interface LineReader.
 *
 * In this application, there is a SimpleLineReader which is always available and a
 * ReadlineLineReader that is only available when complied with libreadline. You shouldn't
 * instantiate that classes directly. Instead, use the LineReader::defaultLineReader() factory.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class LineReader {
    public:
        /**
         * @brief Virtual destructor
         */
        virtual ~LineReader() {}

    public:
        /**
         * @brief Factory function
         *
         * Creates a suitable LineReader instance with prompt @p prompt. If the ReadlineLineReader
         * is available (i.e. if the program is compiled with readline support), that will be used.
         * Otherwise, the SimpleLineReader is used to complete.
         *
         * @param[in] prompt the prompt of the line reader
         * @return a newly allocated LineReader that must be freed by the caller.
         */
        static LineReader *defaultLineReader(const std::string &prompt); /* factory */

    public:
        /**
         * @brief Reads a line
         *
         * Provides the interface to the user to read one single line.
         *
         * @param[in] prompt if not @c NULL, the given prompt will be used. Otherwise, the default prompt
         *            is used.
         * @return the line that has just been read
         */
        virtual std::string readLine(const char *prompt = NULL) = 0;

        /**
         * @brief Returns the prompt
         *
         * @return the prompt
         */
        virtual std::string getPrompt() const = 0;

        /**
         * @brief Checks if we are at the end of input
         *
         * End of input means that the user pressed C-d.
         *
         * @return @c true if we are at the end, @c false otherwise
         */
        virtual bool eof() const = 0;

        /**
         * @brief Reads the history from file @p file.
         *
         * @param[in] file the history file that should be read
         * @exception IOError if reading the history failed
         * @see haveHistory()
         */
        virtual void readHistory(const std::string &file)
        throw (core::IOError) = 0;

        /**
         * @brief Writes the history to @p file
         *
         * @param[in] file the history file that should be written
         * @exception IOError if writing the history failed
         * @see haveHistory()
         */
        virtual void writeHistory(const std::string &file)
        throw (core::IOError) = 0;

        /**
         * @brief Checks if the current implementation has history support
         *
         * If the history support is missing, readHistory() and writeHistory() don't do anything.
         *
         * @return @c true if we have history support, @c false otherwise
         */
        virtual bool haveHistory() const = 0;

        /**
         * @brief Checks if the current implementation has completion support
         *
         * If the completion support is missing, setCompletor() does not have any effect.
         *
         * @return @c true if we have completion support, @c false otherwise
         * @see setCompletor()
         */
        virtual bool haveCompletion() const = 0;

        /**
         * @brief Sets the completor to @p comp
         *
         * @param[in] comp the completor that should be used
         * @see haveCompletion()
         */
        virtual void setCompletor(Completor *comp) = 0;
};

/* }}} */
/* Abstract base class for line readers {{{ */

/**
 * @class AbstractLineReader cli/io.h
 * @brief Implementation of LineReader that safes typing
 *
 * This is just a base class for the SimpleLineReader and the ReadlineLineReader to save some typing.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class AbstractLineReader : public LineReader {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of an AbstractLineReader.
         *
         * @param[in] prompt the default prompt of the line reader.
         */
        AbstractLineReader(const std::string &prompt);

    public:
        /// @copydoc LineReader::getPrompt()
        std::string getPrompt() const;

        /// @copydoc LineReader::eof()
        bool eof() const;

        /// @copydoc LineReader::readHistory()
        void readHistory(const std::string &file)
        throw (core::IOError);

        /// @copydoc LineReader::writeHistory()
        void writeHistory(const std::string &file)
        throw (core::IOError);

        /// @copydoc LineReader::haveHistory()
        bool haveHistory() const;

        /// @copydoc LineReader::haveCompletion()
        bool haveCompletion() const;

        /// @copydoc LineReader::setCompletor()
        void setCompletor(Completor *comp);

    protected:
        /**
         * @brief Sets the EOF flag
         *
         * @param[in] eof @c true if the end of the file is reached, @c false otherwise
         */
        void setEof(bool eof);

    private:
        std::string m_prompt;
        bool m_eof;
};

/* }}} */

} // end namespace cli
} // end namespace usbprog

#endif /* IO_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
