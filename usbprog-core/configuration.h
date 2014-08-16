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
 * @file configuration.h
 * @brief Contains the core configuration
 *
 * This file just contains a container class with getters and setters.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <string>
#include <ostream>

namespace usbprog {
namespace core {

/* Configuration {{{ */

/**
 * @brief The application configuration
 *
 * Usually, this configuration is used as base class where the concrete implementation adds
 * some properties.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */
class Configuration
{
public:
    /**
     * @brief Returns the data directory of USBprog
     *
     * @return the data directory
     */
    std::string getDataDir() const;

    /**
     * @brief Sets the data directory of USBprog
     *
     * @param[in] dir the new data directory
     */
    void setDataDir(const std::string &dir);

    /**
     * @brief Checks if debugging is enabled
     *
     * @return @c true if debugging is enabled, @c false otherwise
     */
    bool getDebug() const;

    /**
     * @brief Enables/disables debugging
     *
     * @param[in] debug @c true if debugging messagtes should be enabled, @c false otherwise.
     */
    void setDebug(bool debug);

    /**
     * @brief Checks if we're in offline mode
     *
     * @return @c true if offline mode is enabled, @c false otherwise.
     */
    bool isOffline() const;

    /**
     * @brief Enables/disables offline mode
     *
     * @param[in] offline @c true if the offline mode should be enabled, @c false otherwise.
     */
    void setOffline(bool offline);

    /**
     * @brief Returns the index URL
     *
     * This is the URL where the <tt>versions.xml</tt> file is located.
     *
     * @return the index URL
     */
    std::string getIndexUrl() const;

    /**
     * @brief Sets the index URL.
     *
     * @param[in] url the new index URL
     */
    void setIndexUrl(const std::string &url);

    /**
     * @brief Dumps the configuration to @p stream
     *
     * This function is for debugging.
     *
     * @param[in,out] stream the stream where the configuration should be written to.
     */
    virtual void dumpConfig(std::ostream &stream);

protected:
    Configuration();
    virtual ~Configuration();

private:
    std::string m_dataDir;
    bool m_debug;
    bool m_offline;
    std::string m_indexUrl;
};

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* CONFIGURATION_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
