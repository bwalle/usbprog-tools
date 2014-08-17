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
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <usbprog-core/inifile.h>

namespace usbprog {
namespace core {

/* IniFile {{{ */

IniFile::IniFile(const std::string &fileName)
    : m_fileName(fileName)
{}

void IniFile::readFile()
{
    std::ifstream file(m_fileName.c_str());

    if (!file)
        throw IOError("Cannot open the specified ini file " + m_fileName + ".");

    std::string line;

    while ( std::getline(file, line) ) {
        size_t equal = line.find("=");
        if (line.find("#") == 0 || equal == std::string::npos)
            continue;

        m_map[line.substr(0, equal)] = line.substr(equal+1, line.length());
    }
}


std::string IniFile::getValue(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator result = m_map.find(key);

    if (result == m_map.end())
        return std::string();
    else
        return (*result).second;
}


int IniFile::getIntValue(const std::string &key) const
{
    std::map<std::string, std::string>::const_iterator result = m_map.find(key);

    if (result == m_map.end())
        return 0;
    else {
        int resultNumber;

        std::istringstream inStream((*result).second);
        inStream >> resultNumber;

        return resultNumber;
    }
}


bool IniFile::isKeyAvailable(const std::string &key) const
{
    return m_map.find(key) != m_map.end();
}

/* }}} */

} // end namespace core
} // end namespace usbprog


// vim: set sw=4 ts=4 et:
