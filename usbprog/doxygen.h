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
 * @defgroup usbprog USBprog function library
 *
 * @brief Firmwarepool and everything around
 *
 * Together with the classes in the group <i>usbprog-core</i> this group contains the whole
 * functionality that is needed to upload a new firmware to the USBprog hardware.
 *
 * The separation between <i>usbprog-core</i> and <i>usbprog</i> is needed because of dependencies:
 * While <i>usbprog</i> depends only on <i>libusb</i>, that component depends on <i>Qt</i>
 * and on <i>boost</i>.
 *
 * Basically following functions are included in this library:
 *
 *  - firmware downloading
 *  - parsing the index XML file
 *  - system information
 */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
