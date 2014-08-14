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
#include <cstdio>
#include <cstring>
#include <cstdarg>

#include <usbprog-core/debug.h>

namespace usbprog {
namespace core {

/* Debug {{{ */

/* -------------------------------------------------------------------------- */
Debug *Debug::m_instance = NULL;

/* -------------------------------------------------------------------------- */
Debug *Debug::debug()
{
    if (!m_instance)
        m_instance = new Debug();

    return m_instance;
}

/* -------------------------------------------------------------------------- */
Debug::Debug()
    : m_debuglevel(DL_NONE)
    , m_handle(stderr)
{}

/* -------------------------------------------------------------------------- */
void Debug::setLevel(Debug::Level level)
{
    m_debuglevel = level;
}

/* -------------------------------------------------------------------------- */
void Debug::setFileHandle(FILE *handle)
{
    if (!handle)
        m_handle = stderr;
    else
        m_handle = handle;
}

/* -------------------------------------------------------------------------- */
FILE *Debug::getFileHandle() const
{
    return m_handle;
}

/* -------------------------------------------------------------------------- */
void Debug::dbg(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_DEBUG, msg, valist);
    va_end(valist);
}

/* -------------------------------------------------------------------------- */
void Debug::info(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_INFO, msg, valist);
    va_end(valist);
}

/* -------------------------------------------------------------------------- */
void Debug::trace(const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(DL_TRACE, msg, valist);
    va_end(valist);
}

/* -------------------------------------------------------------------------- */
void Debug::msg(Debug::Level level, const char *msg, ...)
{
    va_list valist;

    va_start(valist, msg);
    vmsg(level, msg, valist);
    va_end(valist);
}

/* -------------------------------------------------------------------------- */
void Debug::vmsg(Debug::Level level, const char *msg, std::va_list list)
{
    // if the global debug level is too small, then just do nothing
    if (level < m_debuglevel)
        return;

    size_t len = strlen(msg) + 20;
    char *newmsg = new char[len];
    if (!newmsg)
        return;
    memset(newmsg, 0, len);

    // prepend dump level
    switch (level) {
        case DL_TRACE:
            strcat(newmsg, "TRACE: ");
            break;

        case DL_INFO:
            strcat(newmsg, "INFO: ");
            break;

        case DL_DEBUG:
            strcat(newmsg, "DEBUG: ");
            break;

        case DL_NONE:
            break;
    }
    strcat(newmsg, msg);

    // append '\n' if there's no one at the end
    len = strlen(newmsg);
    if (newmsg[len-1] != '\n') {
        newmsg[len++] = '\n';
        newmsg[len] = '\0';
    }

    vfprintf(m_handle, newmsg, list);
    fflush(m_handle);

    delete[] newmsg;
}

/* -------------------------------------------------------------------------- */
Debug::Level Debug::getLevel() const
{
    return m_debuglevel;
}

/* -------------------------------------------------------------------------- */
bool Debug::isDebugEnabled() const
{
    return m_debuglevel < DL_NONE;
}

/* }}} */

} // end namespace core
} // end namespace usbprog

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
