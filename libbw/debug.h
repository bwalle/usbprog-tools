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
#ifndef DEBUG_H
#define DEBUG_H

/**
 * @file debug.h
 * @brief Debug functionality
 *
 * This file contains debug functionality.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#include <cstdio>
#include <cstdarg>

/* Macros {{{ */

/**
 * @brief Writes a debug message with a specified level
 *
 * Example:
 *
 * @code
 * BW_DEBUG(bw::Debug::DL_INFO, "Message: %d", 5);
 * @endcode
 *
 * @param[in] level the debugging level
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see BW_DEBUG_DBG(), BW_DEBUG_INFO(), BW_DEBUG_TRACE()
 */
#define BW_DEBUG(level, ...) \
    bw::Debug::debug()->msg(level, __VA_ARGS__)

/**
 * @brief Writes a debug message (debug level)
 *
 * Example:
 *
 * @code
 * BW_DEBUG_DBG("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see BW_DEBUG_INFO(), BW_DEBUG_TRACE()
 */
#define BW_DEBUG_DBG(...) \
    bw::Debug::debug()->dbg(__VA_ARGS__)

/**
 * @brief Writes a debug message (info level)
 *
 * Example:
 *
 * @code
 * BW_DEBUG_INFO("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see BW_DEBUG_DBG(), BW_DEBUG_TRACE()
 */
#define BW_DEBUG_INFO(...) \
    bw::Debug::debug()->info(__VA_ARGS__)

/**
 * @brief Writes a debug message (trace level)
 *
 * Example:
 *
 * @code
 * BW_DEBUG_DBG("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see BW_DEBUG_INFO(), BW_DEBUG_DBG()
 */
#define BW_DEBUG_TRACE(...) \
    bw::Debug::debug()->trace(__VA_ARGS__)

/* }}} */


namespace bw {

/* Debugging {{{ */

/**
 * @class Debug debug.h libbw/debug.h
 * @brief Debugging singleton class
 *
 * This is the class used for debugging in the program. The class is a
 * singleton. Example usage for initialisation:
 *
 * @code
 * FILE *debug_fp = fopen("debug.log", "wa");
 * if (!debug_fp) {
 *     perror("Cannot open debug file");
 *     return;
 * }
 *
 * Debug *debugger = Debug::debug();
 * debugger->setFileHandle(debug_fp);
 * debugger->setLevel(Debug::DL_INFO);
 * @endcode
 *
 * And then in the application:
 *
 * @code
 * Debug::debug()->dbg("Debugging message, bla=%d\n", bla);
 * @endcode
 *
 * Currently the class supports only one debugging file handle, by default the
 * standard error console but that can also be a file. See setFileHandle().
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Debug {
    public:
        /**
         * @brief Debug level.
         *
         * This enumeration represents the debug levels. The number
         * representation contains gaps to that immediate levels can be
         * introduced without breaking binary compatibility.
         */
        enum Level {
            DL_TRACE    = 0,            /**< tracing, very verbose */
            DL_DEBUG    = 10,           /**< debugging statements */
            DL_INFO     = 20,           /**< informational messages that can be presented to
                                             the average user */
            DL_NONE     = 100           /**< no debugging at all, be silent */
        };

    public:
        /**
         * @brief Singleton getter
         *
         * This function returns the only instance of Debug.
         *
         * @return a pointer to the only debug instance.
         */
        static Debug *debug();

        /**
         * @brief Print a debugging message
         *
         * This is a convenience method for msg() with Debug::DL_DEBUG. After
         * @p msg, the function takes a variable number of arguments.
         *
         * @param[in] msg the message format string in printf() style
         */
        void dbg(const char *msg, ...);

        /**
         * @brief Print a info message
         *
         * This is a convenience method for msg() with Debug::DL_INFO. After
         * @p msg, the function takes a variable number of arguments.
         *
         * @param[in] msg the message format string in printf() style
         */
        void info(const char *msg, ...);

        /**
         * @brief Print a trace message
         *
         * This is a convenience method for msg() with Debug::DL_TRACE. After
         * @p msg, the function takes a variable number of arguments.
         *
         * @param[in] msg the message format string in printf() style
         */
        void trace(const char *msg, ...);

        /**
         * @brief Print a debug string
         *
         * This is a convenience method for dbg(const char *) that takes a
         * std::string.
         *
         * @param[in] s the string that should be printed
         */
        void dbg(const std::string &s);

        /**
         * @brief Print a info string
         *
         * This is a convenience method for info(const char *) that takes a
         * std::string.
         *
         * @param[in] s the string that should be printed
         */
        void info(const std::string &s);

        /**
         * @brief Print a tracing string
         *
         * This is a convenience method for trace(const char *) that takes a
         * std::string.
         *
         * @param[in] s the string that should be printed
         */
        void trace(const std::string &s);

        /**
         * @brief Prints a general debug message
         *
         * This is the general function that can be called instead of dbg(),
         * trace() or info().
         *
         * @param[in] level the debug level (see Debug::Level)
         * @param[in] msg the printf()-like format string for the message
         */
        void msg(Debug::Level level, const char *msg, ...);

        /**
         * @brief Prints a general debug message (vfprintf()-style)
         *
         * This is the function that is called internally by all other
         * functions.
         *
         * @param[in] level the debug level (see Debug::Level)
         * @param[in] msg the printf()-like format string for the message
         * @param[in] args the arguments to @p msg, consult a C book if you
         *            don't know how to deal with that
         */
        void vmsg(Debug::Level level, const char *msg, std::va_list args);

        /**
         * @brief Set the debug level
         *
         * Sets the debug level. All messages with a higher debug level (and,
         * of course, the same debug level) as @p level are printed and/or
         * logged to the file handle.
         *
         * @param[in] level the new debug level
         */
        void setLevel(Debug::Level level);

        /**
         * @brief Returns the current debug level
         *
         * Returns the current debug level.
         *
         * @return the debug level, see Debug::Level
         */
        Debug::Level getLevel() const;

        /**
         * @brief Checks if debugging is enabled at all
         *
         * Convenience method that checks if debugging is enabled at all. Use
         * that function if it's a high overhead to generate the arguments
         * that are needed to call the debug functions. Example:
         *
         * @code
         * if (Debug::debug()->isDebugEnabled()) {
         *     std::string dbgstr = object->toString(); // very time consuming
         *     Debug::debug()->dbg("object: %s\n", dbgstr.c_str());
         * }
         * @endcode
         *
         * @return @c true if debugging is enabled, @c false otherwise
         */
        bool isDebugEnabled() const;

        /**
         * @brief Set the file handle for output
         *
         * Sets the file handle that is used for debugging output. The default
         * is @c stderr (from @c cstdio). See the documentation of the class
         * Debug to see an example how that function can be used to write to a
         * file.
         *
         * @param[in] handle the file handle
         */
        void setFileHandle(FILE *handle);

        /**
         * @brief Returns the file handle
         *
         * Returns the file handle that has been set with setFileHandle(). The
         * default is @c stderr from @c cstdio.
         *
         * @return the file handle of type @c FILE
         */
        FILE *getFileHandle() const;

    protected:
        Debug();

    private:
        static Debug *m_instance;

    private:
        Level m_debuglevel;
        FILE *m_handle;
};

/* }}} */

} // end namespace bw

#endif /* DEBUG_H */

// :tabSize=4:indentSize=4:noTabs=true:mode=c++:folding=explicit:collapseFolds=1:maxLineLen=100:
