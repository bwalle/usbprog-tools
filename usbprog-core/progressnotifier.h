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
 * @file progressnotifier.h
 * @ingroup core
 * @brief Contains an interface to notify the application about progress
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#ifndef PROGRESSNOTIFIER_H
#define PROGRESSNOTIFIER_H

namespace usbprog {
namespace core {

/* ProgressNotifier {{{ */

/**
 * @brief Notify the application about progress for long operations
 *
 * This is a simple interface that the application can implement to get notified about
 * the progress of long operations done by USBprog.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class ProgressNotifier {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~ProgressNotifier() {}

public:
    /**
     * @brief Gets called regularly
     *
     * This member function gets called regulary to notify the caller about the
     * progress. The percentage value can be computed by the formula
     *
     * <tt>percentage = now * 100 / total</tt>
     *
     * The function should never throw.
     *
     * @param[in] total the total value
     * @param[in] now the current value (must be less or equal to now)
     * @return the return value is ignored and will be removed in the future (which means you
     *         should return @c true when implementing the interface and ignore it when
     *         calling that interface)
     */
    virtual int progressed(double total, double now) = 0;

    /**
     * @brief Gets called once when the operation has finished
     *
     * This function gets called once after the operation has finished. For example if
     * a terminal application prints a progress bar with hashes, that hook can be used to
     * print a newline.
     */
    virtual void finished() = 0;
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* PROGRESSNOTIFIER_H */

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
