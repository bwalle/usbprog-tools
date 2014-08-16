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

/**
 * @file inifile.h
 * @brief Simple ini parser
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef USBPROG_INIFILE_H
#define USBPROG_INIFILE_H

#include <string>
#include <map>

#include <usbprog-core/error.h>

namespace usbprog {
namespace core {

/* IniFile {{{ */

/**
 * @brief Simple configuration file parser
 *
 * Simple key-value configuration file parser without sections.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class IniFile {
public:
    /**
     * @brief Constructor
     *
     * Creates a new ini parser. The constructor doesn't check whether @p fileName exists.
     * The user must call readFile() before using any getter methods like getValue(),
     * getIntValue() or isKeyAvailable().
     *
     * @param[in] fileName the name of the file to parse
     */
    IniFile(const std::string &fileName);

    /**
     * @brief Reads the file whose file name has been set in the constructor.
     *
     * @exception IOError if reading the file failed.
     */
    void readFile();

    /**
     * @brief Returns the string value of @p key
     *
     * Don't forget to call readFile() before!
     *
     * @param[in] key the name of the key
     * @return the value or the empty string (<tt>""</tt>) if the key doesn't exist
     */
    std::string getValue(const std::string &key) const;

    /**
     * @brief Returns the integer value of @p key
     *
     * Don't forget to call readFile() before!
     *
     * @param[in] key the name of the key
     * @return the integer value or 0 if the key doesn't exist
     */
    int getIntValue(const std::string &key) const;

    /**
     * @brief Checks if @p key exists
     *
     * @param[in] key the name of the key
     * @return @c true if the key exists, @c false otherwise.
     */
    bool isKeyAvailable(const std::string &key) const;

private:
    std::string m_fileName;
    std::map<std::string, std::string> m_map;
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_INIFILE_H */

// vim: set sw=4 ts=4 et:
