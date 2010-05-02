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
#ifndef SLEEPER_H
#define SLEEPER_H

namespace usbprog {
namespace core {

/* Sleeper {{{ */

class Sleeper
{
    public:
        virtual ~Sleeper() {}

    public:
        virtual void sleep(int ms) = 0;
};

/* }}} */
/* BlockingSleeper {{{ */

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
