/* {{{
 * Copyright (c) 2008-2010, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */

/**
 * @file completion.h
 * @brief Shell-like command line reader with completion
 *
 * This file contains a shell-like command reader with completion.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#ifndef COMPLETION_H
#define COMPLETION_H

#include "bwerror.h"
#include <vector>

namespace bw {

/* Interface for completors {{{ */

/**
 * @class Completor completion.h libbw/completion.h
 * @brief Interface for auto-completion
 *
 * This class represents an interface that one must implement to get
 * auto-completion.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Completor {
    public:
        /**
         * @brief Destructor
         *
         * Deletes a Completor object.
         */
        virtual ~Completor() {}

    public:
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
        virtual std::vector<std::string> complete(const std::string &text,
                                                  const std::string &full_text,
                                                  size_t            start_idx,
                                                  ssize_t           end_idx) = 0;
};

/* }}} */
/* Interface for a linereader {{{ */

/**
 * @class LineReader completion.h libbw/complection.h
 * @brief Interface for a line reader
 *
 * This class represents an interface for a line reader. A line reader is a
 * class that reads lines like a shell, including tab-complection. Completion
 * is optional.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class LineReader {
    public:
        /**
         * @brief Destructor
         *
         * Destroys a LineReader object.
         */
        virtual ~LineReader() {}

    public:
        /**
         * @brief Returns a suitable LineReader implementation
         *
         * Returns a suitable LineReader implementation on the system. This is
         * a factory method.
         *
         * @param[in] prompt the prompt for the LineReader
         * @return the LineReader implementation. The caller is responsible to
         *         free the LineReader object.
         */
        static LineReader *defaultLineReader(const std::string &prompt); /* factory */

    public:
        /**
         * @brief Reads a line
         *
         * Reads a whole line. Displays the supplied @p prompt.
         *
         * @param[in] prompt the prompt to display. You can pass @c NULL if
         *            you want to use the default prompt (see
         *            defaultLineReader()).
         * @return the read line
         */
        virtual std::string readLine(const char *prompt = NULL) = 0;

        /**
         * @brief Returns the prompt
         *
         * Returns the prompt of the LineReader.
         *
         * @return the prompt
         */
        virtual std::string getPrompt() const = 0;

        /**
         * @brief Checks for the end of file
         *
         * Checks if the end of file character (Ctrl-D on Unix) has been
         * received. Normally the application should terminate then.
         *
         * @return @c true if the end of line character has been received, @c
         *         false otherwise
         */
        virtual bool eof() const = 0;

        /**
         * @brief Reads the history from file
         *
         * Reads the history information from file. Normally that function is
         * called on startup of the application.
         *
         * @param[in] file the history file that should be read
         * @exception IOError if the file cannot be opened for reading
         */
        virtual void readHistory(const std::string &file)
            throw (IOError) = 0;

        /**
         * @brief Write history to file
         *
         * Writes the history to a file. Normally that function is called on
         * shutdown of the application.
         *
         * @param[in] file the file where the history should be written to
         * @exception IOError if the file cannot be opened or written to
         */
        virtual void writeHistory(const std::string &file)
            throw (IOError) = 0;

        /**
         * @brief Checks if the implementation has history functionality.
         *
         * Checks if the implementation has history functionality.
         *
         * @return @c true if the implementation has history functionality, @c
         *         false otherwise
         */
        virtual bool haveHistory() const = 0;

        /**
         * @brief Checks if the implementation has a completion
         *
         * Checks if the implementation has completion functionality.
         *
         * @return @c true if the implementation has completion functionality,
         *         @c false otherwise
         */
        virtual bool haveCompletion() const = 0;

        /**
         * @brief Sets the completor
         *
         * Sets the completor object. See the description of Completor for how
         * to implement a completor.
         *
         * The memory management of @p completor has to be done by the
         * application and is not done by the implementation of the
         * LineReader.
         *
         * @param[in] comp the completor object
         */
        virtual void setCompletor(Completor *comp) = 0;

        /**
         * @brief Checks if the line is editable
         *
         * Checks if the line of the LineReader can be set by the application.
         * In other words, checks if editLine() is available.
         *
         * @return @c true if the line is editable, @c false otherwise
         */
        virtual bool canEditLine() const = 0;

        /**
         * @brief Edits the current line
         *
         * Sets the contents of the current line to @p oldLine, lets the user
         * edit the line and finally returns the new contents as return value.
         *
         * @param[in] oldLine the inital contents of the line or @c NULL if
         *            the line should be initially empty
         * @return the contents of the new line after the user has edited the
         *         line
         */
        virtual std::string editLine(const char *oldLine) = 0;
};

/* }}} */
/* Abstract base class for line readers {{{ */

/**
 * @class AbstractLineReader completion.h libbw/completion.h
 * @brief Abstract base class for LineReader implementations
 *
 * This is a abstract base class for line reader implementations.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class AbstractLineReader : public LineReader {
    public:
        /**
         * @brief Constructor
         *
         * Creates a new AbstractLineReader instance.
         *
         * @param[in] prompt the prompt of the line reader
         */
        AbstractLineReader(const std::string &prompt);

    public:

        /**
         * @copydoc LineReader::getPrompt()
         */
        std::string getPrompt() const;

        /**
         * @copydoc LineReader::eof()
         */
        bool eof() const;

        /**
         * @brief Reads the history
         *
         * Reads the history from file
         *
         * @param[in] file the history file
         * @exception IOError never
         */
        void readHistory(const std::string &file)
            throw (IOError);

        /**
         * @brief Writes the history
         *
         * Does nothing, stub implementation.
         *
         * @param[in] file the file that should be read
         * @exception IOError never
         */
        void writeHistory(const std::string &file)
            throw (IOError);

        /**
         * @copydoc LineReader::haveHistory()
         */
        virtual bool haveHistory() const;

        /**
         * @copydoc LineReader::canEditLine()
         */
        virtual bool canEditLine() const;

        /**
         * @copydoc LineReader::editLine(const char *)
         */
        virtual std::string editLine(const char *oldLine);

        /**
         * @brief Checks if the implementation has completion
         *
         * Returns always @c false.
         *
         * @return @c false
         */
        bool haveCompletion() const;

        /**
         * @brief Sets the completor
         *
         * Does nothing, just throws away the @p comp.
         *
         * @param[in] comp the completor
         */
        void setCompletor(Completor *comp);

    protected:
        /**
         * @brief Sets the EOF status
         *
         * Sets the EOF status that is returned by eof().
         *
         * @param[in] eof the eof status
         */
        void setEof(bool eof);

    private:
        std::string m_prompt;
        bool m_eof;
};

/* }}} */

} // end namespace bw

#endif /* COMPLETION_H */

// :tabSize=4:indentSize=4:noTabs=true:mode=c++:folding=explicit:collapseFolds=1:maxLineLen=100:
