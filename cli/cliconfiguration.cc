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
#include "cliconfiguration.h"

CliConfiguration *CliConfiguration::m_instance = NULL;

/* -------------------------------------------------------------------------- */
CliConfiguration::CliConfiguration()
{}

/* -------------------------------------------------------------------------- */
CliConfiguration::~CliConfiguration()
{}

/* -------------------------------------------------------------------------- */
CliConfiguration* CliConfiguration::config()
{
    if (!m_instance)
        m_instance = new CliConfiguration();

    return m_instance;
}

/* -------------------------------------------------------------------------- */
void CliConfiguration::setHistoryFile(const std::string &history)
{
    m_historyFile = history;
}

/* -------------------------------------------------------------------------- */
std::string CliConfiguration::getHistoryFile() const
{
    return m_historyFile;
}

/* -------------------------------------------------------------------------- */
void CliConfiguration::setBatchMode(bool batch)
{
    m_batchMode = batch;
}

/* -------------------------------------------------------------------------- */
bool CliConfiguration::getBatchMode() const
{
    return m_batchMode;
}

/* -------------------------------------------------------------------------- */
void CliConfiguration::dumpConfig(std::ostream &stream)
{
    Configuration::dumpConfig(stream);
    stream << "history     = " << m_historyFile  << std::endl
           << "batch mode  = " << m_batchMode    << std::endl;
}

// vim: set sw=4 ts=4 et: :collapseFolds=1:
