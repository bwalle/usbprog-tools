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
 * @defgroup core USBprog core functions
 *
 * All classes and functions in that library are part of the so-called "USBprog core"
 * library. That contains utility classes for platform abstraction as well as the
 * calls to libusb to program the USBprog.
 *
 * The special thing about this library is that it has almost no dependencies: It only
 * requires libusb (the usbpp abstraction layer, to be more specific).
 *
 * All classes and functions are in the <tt>usbprog::core</tt> namespace.
 */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
