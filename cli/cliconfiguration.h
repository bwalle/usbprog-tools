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
 * @file cliconfiguration.h
 * @brief Configuration for the CLI program
 *
 * This file contains the class CliConfiguration.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */

#ifndef CLICONFIGURATION_H
#define CLICONFIGURATION_H

#include <iostream>
#include <string>
#include <ostream>

#include <usbprog-core/configuration.h>

namespace usbprog {
namespace cli {

/* CliConfiguration {{{ */

/**
 * @class CliConfiguration cli/cliconfiguration.h
 * @brief Configuration properties for the CLI
 *
 * This class extends the core::Configuration with CLI-specific configuration properties.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup cli
 */
class CliConfiguration : public core::Configuration
{
public:
    /**
     * @brief Singleton accessor
     *
     * @return a reference to the only CliConfiguration object
     */
    static CliConfiguration &config();

    /**
     * @brief Sets the history file
     *
     * @param[in] history the file name of the history file (which is managed by libreadline)
     */
    void setHistoryFile(const std::string &history);

    /**
     * @brief Returns the history file
     *
     * @return the history file
     */
    std::string getHistoryFile() const;

    /**
     * @brief Set whether we are using batch mode
     *
     * If the program is running in batch mode, it just executes the command passed by the
     * command line. It doesn't accept interactive input.
     *
     * @param[in] batch @c true if the program is operating in batch mode, @c false otherwise.
     */
    void setBatchMode(bool batch);

    /**
     * @brief Checks if the program is using batch mode
     *
     * @return @c true if the program is operating in batch mode, @c false otherwise.
     */
    bool getBatchMode() const;

    /**
     * @copydoc core::Configuration::dumpConfig()
     */
    virtual void dumpConfig(std::ostream &stream);

protected:
    CliConfiguration();
    virtual ~CliConfiguration();

private:
    static CliConfiguration *m_instance;
    bool m_batchMode;
    std::string m_historyFile;
};

/* }}} */

} // end namespace cli
} // end namespace usbprog

#endif /* CLICONFIGURATION_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
