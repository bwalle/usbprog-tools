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
 * @file guiconfiguration.h
 * @brief Configuration for the GUI program.
 *
 * This file contains the class GuiConfiguration.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */

#ifndef GUICONFIGURATION_H
#define GUICONFIGURATION_H

#include <iostream>
#include <string>
#include <ostream>

#include <usbprog-core/configuration.h>

namespace usbprog {
namespace gui {

/* GuiConfiguration {{{ */

/**
 * @class GuiConfiguration gui/guiconfiguration.h
 * @brief Configuration properties for the GUI.
 *
 * This class extends the core::Configuration with CLI-specific configuration properties.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup gui
 */
class GuiConfiguration : public core::Configuration
{
    public:
        /**
         * @brief Singleton accessor
         *
         * @return a reference to the only CliConfiguration object
         */
        static GuiConfiguration &config();

        /**
         * @copydoc core::Configuration::dumpConfig()
         */
        virtual void dumpConfig(std::ostream &stream);

    protected:
        GuiConfiguration();
        virtual ~GuiConfiguration();

    private:
        static GuiConfiguration *m_instance;
};

/* }}} */

} // end namespace gui
} // end namespace usbprog


#endif /* GUICONFIGURATION_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
