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
#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include <map>
#include <stdexcept>
#include <cstdarg>

/* Macros {{{ */

#define USBPROG_DEBUG(level, ...) \
    usbprog::core::Debug::debug()->msg(level, __VA_ARGS__)

#define USBPROG_DEBUG_DBG(...) \
    usbprog::core::Debug::debug()->dbg(__VA_ARGS__)

#define USBPROG_DEBUG_INFO(...) \
    usbprog::core::Debug::debug()->info(__VA_ARGS__)

#define USBPROG_DEBUG_TRACE(...) \
    usbprog::core::Debug::debug()->trace(__VA_ARGS__)

/* }}} */

namespace usbprog {
namespace core {

/* Debugging {{{ */

class Debug {
    public:
        enum Level {
            DL_TRACE    = 0,
            DL_DEBUG    = 10,
            DL_INFO     = 20,
            DL_NONE     = 100
        };

    public:
        static Debug *debug();

        void dbg(const char *msg, ...);
        void info(const char *msg, ...);
        void trace(const char *msg, ...);
        void msg(Debug::Level level, const char *msg, ...);
        void vmsg(Debug::Level level, const char *msg, std::va_list args);

        void setLevel(Debug::Level level);
        Debug::Level getLevel() const;
        bool isDebugEnabled() const;
        void setFileHandle(FILE *handle);
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
