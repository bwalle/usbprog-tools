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
#ifndef CLICONFIGURATION_H
#define CLICONFIGURATION_H

#include <iostream>
#include <string>
#include <ostream>

#include <usbprog-core/configuration.h>

class CliConfiguration : public Configuration
{
    public:
        static CliConfiguration *config();

        void setHistoryFile(const std::string &history);
        std::string getHistoryFile() const;

        void setBatchMode(bool batch);
        bool getBatchMode() const;

        virtual void dumpConfig(std::ostream &stream);

    protected:
        CliConfiguration();
        virtual ~CliConfiguration();

    private:
        static CliConfiguration *m_instance;
        bool m_batchMode;
        std::string m_historyFile;
};

#endif /* CLICONFIGURATION_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
