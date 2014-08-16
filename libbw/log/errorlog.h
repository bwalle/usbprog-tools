/* {{{
 * Copyright (c) 2011, Bernhard Walle <bernhard@bwalle.de>
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
#ifndef LIBBW_LOG_ERRORLOG_H_
#define LIBBW_LOG_ERRORLOG_H_

#include <cstdarg>
#include <cstdlib>
#include <string>
#include <sstream>

#include <libbw/compiler.h>

/* Macros {{{ */

/**
 * \brief Writes an error log message with a specified level
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR(bw::Errorlog::LS_EMERG, "Message: %d", 5);
 * \endcode
 *
 * \param[in] level the error log level
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR_EMERG(), BW_ERROR_ALERT(), BW_ERROR_CRIT(), BW_ERROR_ERR(), BW_ERROR_WARNING()
 */
#define BW_ERROR(level, ...)                                        \
    do {                                                            \
        bw::Errorlog *log = bw::Errorlog::instance();               \
        if (log)                                                    \
            log->log(level, __VA_ARGS__);                           \
    } while (0)

/**
 * \brief Writes an error log message with a specified level (C++ stream version)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR(bw::Errorlog::LS_EMERG, "Message: " << 5);
 * \endcode
 *
 * \param[in] level the error log level
 * \param[in] args a set of stream operations as shown in the example
 * \ingroup log
 * \see BW_ERROR_STREAM_EMERG(), BW_ERROR_STREAM_ALERT(), BW_ERROR_STREAM_CRIT(), BW_ERROR_STREAM_ERR(), BW_ERROR_STREAM_WARNING()
 */
#define BW_ERROR_STREAM(level, args)                                \
    do {                                                            \
        std::ostringstream _oss;                                    \
        _oss << args;                                               \
        bw::Errorlog *log = bw::Errorlog::instance();               \
        if (log)                                                    \
        log->log(level, "%s", _oss.str().c_str());                  \
    } while (0)

/**
 * \brief Writes error log level (emergency level)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_EMERG("Message: %d", 5);
 * \endcode
 *
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR(), BW_ERROR_ALERT(), BW_ERROR_CRIT(), BW_ERROR_ERR(), BW_ERROR_WARNING()
 */
#define BW_ERROR_EMERG(...) \
    BW_ERROR(bw::Errorlog::LS_EMERG, __VA_ARGS__)

/**
 * \brief Writes error log level (emergency level) (C++ streams)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_STREAM_EMERG("Message: %d" << 5);
 * \endcode
 *
 * \param[in] args some stream operations as shown in the example
 * \ingroup log
 */
#define BW_ERROR_STREAM_EMERG(args) \
    BW_ERROR_STREAM(bw::Errorlog::LS_EMERG, args)

/**
 * \brief Writes error log level (alert level)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_ALERT("Message: %d", 5);
 * \endcode
 *
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR(), BW_ERROR_EMERG(), BW_ERROR_CRIT(), BW_ERROR_ERR(), BW_ERROR_WARNING()
 */
#define BW_ERROR_ALERT(...) \
    BW_ERROR(bw::Errorlog::LS_ALERT, __VA_ARGS__)

/**
 * \brief Writes error log level (alert level) (C++ streams)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_STREAM_ALERT("Message: %d" << 5);
 * \endcode
 *
 * \param[in] args some stream operations as shown in the example
 * \ingroup log
 */
#define BW_ERROR_STREAM_ALERT(args) \
    BW_ERROR_STREAM(bw::Errorlog::LS_ALERT, args)

/**
 * \brief Writes error log level (critical level)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_CRIT("Message: %d", 5);
 * \endcode
 *
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR(), BW_ERROR_ALERT(), BW_ERROR_EMERG(), BW_ERROR_ERR(), BW_ERROR_WARNING()
 */
#define BW_ERROR_CRIT(...) \
    BW_ERROR(bw::Errorlog::LS_CRIT, __VA_ARGS__)

/**
 * \brief Writes error log level (critical level) (C++ streams)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_STREAM_CRIT("Message: %d" << 5);
 * \endcode
 *
 * \param[in] args some stream operations as shown in the example
 * \ingroup log
 */
#define BW_ERROR_STREAM_CRIT(args) \
    BW_ERROR_STREAM(bw::Errorlog::LS_CRIT, args)

/**
 * \brief Writes error log level (error level)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_ERR("Message: %d", 5);
 * \endcode
 *
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR(), BW_ERROR_ALERT(), BW_ERROR_CRIT(), BW_ERROR_CRIT(), BW_ERROR_WARNING()
 */
#define BW_ERROR_ERR(...) \
    BW_ERROR(bw::Errorlog::LS_ERR, __VA_ARGS__)

/**
 * \brief Writes error log level (error level) (C++ streams)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_STREAM_ERR("Message: %d" << 5);
 * \endcode
 *
 * \param[in] args some stream operations as shown in the example
 * \ingroup log
 */
#define BW_ERROR_STREAM_ERR(args) \
    BW_ERROR_STREAM(bw::Errorlog::LS_ERR, args)

/**
 * \brief Writes error log level (warning level)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_WARNING("Message: %d", 5);
 * \endcode
 *
 * \param[in] ... the format string and an arbitrary number of arguments.
 * \ingroup log
 * \see BW_ERROR(), BW_ERROR_ALERT(), BW_ERROR_CRIT(), BW_ERROR_ERR(), BW_ERROR_EMERG()
 */
#define BW_ERROR_WARNING(...) \
    BW_ERROR(bw::Errorlog::LS_WARNING, __VA_ARGS__)

/**
 * \brief Writes error log level (emergency level) (C++ streams)
 *
 * Example:
 *
 * \code
 * #include <libbw/log/errorlog.h>
 *
 * BW_ERROR_STREAM_WARNING("Message: %d" << 5);
 * \endcode
 *
 * \param[in] args some stream operations as shown in the example
 * \ingroup log
 */
#define BW_ERROR_STREAM_WARNING(args) \
    BW_ERROR_STREAM(bw::Errorlog::LS_WARNING, args)

/* }}} */

namespace bw {

/* Errorlog {{{ */

/**
 * \brief Simple error logging
 *
 * This is a simple error logging abstraction that provides logging to a C file object or to syslog
 * (on Unix). The most common case is logging to stderr.
 *
 * To use it, you have to configure the errorlog once using the static method Errorlog::configure().
 * After configuration, you can either retrieve the only instance of the errorlog object with
 * Errorlog::instance() or use the macros provided above.
 *
 * The real logging is provided by subclasses of Errorlog. A subclass only need to provide a
 * constructor, a destructor (optional) and an implementation of Errorlog::vlog().
 *
 * To perform the actual logging in the application, use the macros
 *
 *  - BW_ERROR() for generic logs
 *  - BW_ERROR_EMERG() for panic conditions
 *  - BW_ERROR_ALERT() for conditions that should be corrected immediately
 *  - BW_ERROR_CRIT() for critical conditions
 *  - BW_ERROR_ERR() for errors
 *  - BW_ERROR_WARNING() for warning messages
 *
 * On platforms where libbw provides a thread implementation (currently that applies to all
 * platforms that have a pthread implementation which includes Linux, Mac OS and BSDs)
 * the error logging is thread safe, i.e. lines don't get mixed.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup log
 */
class Errorlog {

public:
    /**
     * \brief Enumeration value for Errorlog::configure().
     */
    enum LogMethod {
        LM_FILE,            /**< logging to a C FILE object */
        LM_SYSLOG           /**< logging to syslog on Unix */
    };

    /**
     * \brief Configures the logging in the system.
     *
     * Before calling configure(), no logging is performed at all.
     *
     * If \p method is LM_FILE, then \p option is the name of the file to which the log messages
     * are written to. The special values \c "stdout" and \c "stderr" are supported. Passing \c
     * NULL means \c "stderr".
     *
     * If \p method is LM_SYSLOG, then \p option is the \c ident parameter for openlog(). Please
     * note that LM_SYSLOG is only available on Unix.
     *
     * \param[in] method the log method which can be LM_FILE on all operating systems and
     *            LM_SYSLOG on Unix.
     * \param[in] option the option argument as described above.
     * \return \c true on success and \c false on failure. The only failure cause can be calling
     *         configure() with LM_SYSLOG on Windows.
     */
    static bool configure(enum LogMethod method, const char *option=NULL);

    /**
     * \brief Returns the only instance of Errorlog
     *
     * If you don't call Errorlog::configure() before, \c NULL is returned.
     *
     * \return the instnace of Errorlog
     */
    static Errorlog *instance();

public:
    /**
     * \brief D'tor
     */
    virtual ~Errorlog() {}

public:
    /**
     * \brief Supported severities
     *
     * These severities map 1:1 to syslog severities with the exception that INFO and DEBUG are
     * not provided. Use the Debug class for that case.
     */
    enum Level {
        LS_EMERG,       /**< A panic condition. */
        LS_ALERT,       /**< A condition that should be corrected immediately. */
        LS_CRIT,        /**< Critical conditions. */
        LS_ERR,         /**< Errors */
        LS_WARNING      /**< Warning messages. */
    };

    /**
     * \brief Converts \p level to string
     *
     * The strings are in uppercase. Since the returned strings point to a static string
     * buffer, don't free or modify the returned string.
     *
     * \param[in] level the error level
     * \return a string or \c NULL if \p level is invalid.
     */
    static const char *levelToString(enum Errorlog::Level level);

    /**
     * \brief Logs an emergency error
     *
     * \param[in] msg the printf()-like format string for the message
     */
    void emerg(const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(2, 3);

    /**
     * \brief Logs an alert error
     *
     * \param[in] msg the printf()-like format string for the message
     */
    void alert(const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(2, 3);

    /**
     * \brief Logs a critical error
     *
     * \param[in] msg the printf()-like format string for the message
     */
    void crit(const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(2, 3);

    /**
     * \brief Logs an normal error
     *
     * \param[in] msg the printf()-like format string for the message
     */
    void err(const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(2, 3);

    /**
     * \brief Logs an warning error
     *
     * \param[in] msg the printf()-like format string for the message
     */
    void warning(const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(2, 3);

    /**
     * \brief Prints a general error log message
     *
     * This is the general function that can be called instead of emerg(), alert(),
     * crit(), err() and warning().
     *
     * \param[in] level the error level (see Errorlog::Level)
     * \param[in] msg the printf()-like format string for the message
     */
    void log(Errorlog::Level level, const char *msg, ...)
    BW_COMPILER_PRINTF_FORMAT(3, 4);

    /**
     * \brief Prints a general error log message (vfprintf()-style)
     *
     * This is the function that is called internally by all other functions.
     *
     * \param[in] level the log level (see Errorlog::Level)
     * \param[in] msg the printf()-like format string for the message
     * \param[in] args the arguments to \p msg, consult a C book if you
     *            don't know how to deal with that
     */
    virtual void vlog(Errorlog::Level level, const char *msg, std::va_list args)
    BW_COMPILER_PRINTF_FORMAT(3, 0) = 0;

    /**
     * \brief Prints a general error log message (string version)
     *
     * This is the general function that can be called instead of emerg(), alert(),
     * crit(), err() and warning().
     *
     * \param[in] level the error level (see Errorlog::Level)
     * \param[in] msg a message to print
     */
    void log(Errorlog::Level level, const std::string msg);

private:
    static Errorlog *m_instance;
};

/* }}} */

} // end namespace bw

#endif /* LIBBW_LOG_ERRORLOG_H_ */
