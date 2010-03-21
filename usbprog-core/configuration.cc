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
#include "configuration.h"

/* -------------------------------------------------------------------------- */
Configuration::Configuration()
    : m_debug(false)
    , m_offline(false)
{}

/* -------------------------------------------------------------------------- */
Configuration::~Configuration()
{}

/* -------------------------------------------------------------------------- */
void Configuration::setDataDir(const std::string &dir)
{
    m_dataDir = dir;
}

/* -------------------------------------------------------------------------- */
std::string Configuration::getDataDir() const
{
    return m_dataDir;
}

/* -------------------------------------------------------------------------- */
void Configuration::setDebug(bool debug)
{
    m_debug = debug;
}

/* -------------------------------------------------------------------------- */
bool Configuration::getDebug() const
{
    return m_debug;
}

/* -------------------------------------------------------------------------- */
void Configuration::setOffline(bool offline)
{
    m_offline = offline;
}

/* -------------------------------------------------------------------------- */
bool Configuration::isOffline() const
{
    return m_offline;
}

/* -------------------------------------------------------------------------- */
void Configuration::setIndexUrl(const std::string &url)
{
    m_indexUrl = url;
}

/* -------------------------------------------------------------------------- */
std::string Configuration::getIndexUrl() const
{
    return m_indexUrl;
}

/* -------------------------------------------------------------------------- */
void Configuration::dumpConfig(std::ostream &stream)
{
    stream << "dataDir     = " << m_dataDir  << std::endl
           << "debug       = " << m_debug    << std::endl
           << "offline     = " << m_offline  << std::endl
           << "indexURL    = " << m_indexUrl << std::endl;
}

// vim: set sw=4 ts=4 et: :collapseFolds=1:
