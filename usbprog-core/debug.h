/*
 * (c) 2010, Bernhard Walle <bernhard@bwalle.de>
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
 * @file debug.h
 * @brief Debugging "framework" for USBprog
 *
 * This file contains debugging functions.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include <map>
#include <stdexcept>
#include <cstdarg>

/* Macros {{{ */

/**
 * @brief Writes a debug message with a specified level
 *
 * Example:
 *
 * @code
 * USBPROG_DEBUG(usbprog::core::Debug::DL_INFO, "Message: %d", 5);
 * @endcode
 *
 * @param[in] level the debugging level
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see USBPROG_DEBUG_DBG(), USBPROG_DEBUG_INFO(), USBPROG_DEBUG_TRACE()
 */
#define USBPROG_DEBUG(level, ...) \
    usbprog::core::Debug::debug()->msg(level, __VA_ARGS__)

/**
 * @brief Writes a debug message (debug level)
 *
 * Example:
 *
 * @code
 * USBPROG_DEBUG_DBG("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see USBPROG_DEBUG_INFO(), USBPROG_DEBUG_TRACE()
 */
#define USBPROG_DEBUG_DBG(...) \
    usbprog::core::Debug::debug()->dbg(__VA_ARGS__)

/**
 * @brief Writes a debug message (info level)
 *
 * Example:
 *
 * @code
 * USBPROG_DEBUG_INFO("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see USBPROG_DEBUG_DBG(), USBPROG_DEBUG_TRACE()
 */
#define USBPROG_DEBUG_INFO(...) \
    usbprog::core::Debug::debug()->info(__VA_ARGS__)

/**
 * @brief Writes a debug message (trace level)
 *
 * Example:
 *
 * @code
 * USBPROG_DEBUG_DBG("Message: %d", 5);
 * @endcode
 *
 * @param[in] ... the format string and an arbitrary number of arguments.
 * @ingroup core
 * @see USBPROG_DEBUG_INFO(), USBPROG_DEBUG_DBG()
 */
#define USBPROG_DEBUG_TRACE(...) \
    usbprog::core::Debug::debug()->trace(__VA_ARGS__)

/* }}} */

namespace usbprog {
namespace core {

/* Debugging {{{ */

/**
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
 * @note Consider using the macros USBPROG_DEBUG(), USBPROG_DEBUG_DBG(), USBPROG_DEBUG_INFO() and
 *       USBPROG_DEBUG_TRACE().
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
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
         * @return the only instance of Debug.
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
         * @brief Print a debugging message
         *
         * This is a convenience method for msg() with Debug::DL_DEBUG. After
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

} // end namespace core
} // end namespace usbprog

#endif /* DEBUG_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
