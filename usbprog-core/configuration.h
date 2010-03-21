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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <iostream>
#include <string>
#include <ostream>

#include <boost/utility.hpp>

class Configuration : boost::noncopyable
{
    public:
        void setDataDir(const std::string &dir);
        std::string getDataDir() const;

        void setDebug(bool debug);
        bool getDebug() const;

        void setOffline(bool offline);
        bool isOffline() const;

        void setIndexUrl(const std::string &url);
        std::string getIndexUrl() const;

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


#endif /* CONFIGURATION_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1: