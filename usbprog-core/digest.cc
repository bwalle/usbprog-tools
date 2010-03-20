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
#include <fstream>
#include <sstream>
#include <iomanip>

#include <md5/md5.h>
#include <usbprog-core/digest.h>

/* -------------------------------------------------------------------------- */
#define BUFFERSIZE 2048

/* -------------------------------------------------------------------------- */
MD5Digest::MD5Digest()
{
    m_md5 = new md5_t;
    md5_init(reinterpret_cast<md5_t *>(m_md5));
}

/* -------------------------------------------------------------------------- */
MD5Digest::~MD5Digest()
{
    delete reinterpret_cast<md5_t *>(m_md5);
}

/* -------------------------------------------------------------------------- */
void MD5Digest::process(unsigned char *buffer, size_t len)
{
    md5_process(reinterpret_cast<md5_t *>(m_md5), buffer, len);
}

/* -------------------------------------------------------------------------- */
std::string MD5Digest::end()
{
    char buffer[16];

    md5_finish(reinterpret_cast<md5_t *>(m_md5), buffer);
    std::stringstream ret;
    for (int i = 0; i < 16; i++) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(2) << int(buffer[i]);

        std::string temp = ss.str();
        ret << temp[temp.size()-2] << temp[temp.size()-1];
    }

    return ret.str();
}

/* -------------------------------------------------------------------------- */
bool check_digest(const std::string &file, const std::string &reference,
        Digest::Algorithm da) throw(IOError)
{
    char buffer[BUFFERSIZE];

    if (da != Digest::DA_MD5)
        return false;

    MD5Digest digest;

    std::ifstream fin(file.c_str(), std::ios::binary);
    if (!fin)
        throw IOError("Opening " + file + " failed");

    while (!fin.eof()) {
        fin.read(buffer, BUFFERSIZE);
        if (fin.bad())
            throw("Error while reading data from " + file);

        digest.process(reinterpret_cast<unsigned char *>(buffer),
                fin.gcount());
    }

    fin.close();

    std::string result = digest.end();
    return result == reference;
}

// vim: set sw=4 ts=4 fdm=marker et: :collapseFolds=1:
