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
 * @file sleeper.h
 * @brief Contains an interface and a simple implementation of a sleeper
 *
 * See the documentation of the Sleeper interface for more details about why that abstraction
 * is needed.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef SLEEPER_H
#define SLEEPER_H

namespace usbprog {
namespace core {

/* Sleeper {{{ */

/**
 * @brief Sleeper interface
 *
 * When communicating with the device it's sometimes necessary to sleep for a while.
 * It depends on the application type how that can be implemented. In simple console
 * applications, a call to the system sleep() function is sufficient.
 *
 * However, in GUI applications that means that the GUI hangs. It's possible to work around
 * that problem by implementing the device communication in an extra thread. However, a much
 * more simple solution exists in most GUI toolkits: Keeping the event loop alive.
 *
 * So that abstraction was made to allow each application to implement their sleep
 * function.
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Sleeper
{
public:
    /**
     * @brief The virtual destructor.
     */
    virtual ~Sleeper() {}

public:
    /**
     * @brief The real sleep function that does nothing but sleep
     *
     * @param[in] ms the amount of time in milliseconds that the function should sleep
     */
    virtual void sleep(int ms) = 0;
};

/* }}} */
/* BlockingSleeper {{{ */

/**
 * @brief Blocking implementation of sleep()
 *
 * This implementation of Sleeper just sleeps by calling sleep(). It works on POSIX platforms
 * as well as on Microsoft Windows.
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class BlockingSleeper : public Sleeper
{
public:
    virtual void sleep(int ms);
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* SLEEPER_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
