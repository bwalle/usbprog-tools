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
#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

#include <list>
#include <map>
#include <string>
#include <vector>

enum OptionType {
    OT_INVALID,
    OT_STRING,
    OT_INTEGER,
    OT_FLAG
};

class OptionValue {
    public:
        OptionValue();

    public:
        void setType(OptionType type);
        OptionType getType() const;

        void setString(const std::string &string);
        std::string getString() const;

        void setFlag(bool flag);
        bool getFlag() const;

        void setInteger(int value);
        int getInteger() const;

    private:
        OptionType      m_type;
        int             m_integer;
        std::string     m_string;
        bool            m_flag;
};

class Option {
    public:
        Option();
        Option(const std::string &name, char letter,
                OptionType type = OT_FLAG,
                const std::string &description = "");

    public:
        void setLongName(const std::string &name);
        std::string getLongName() const;

        void setLetter(char letter);
        char getLetter() const;

        void setType(OptionType type);
        OptionType getType() const;

        void setDescription(const std::string &description);
        std::string getDescription() const;

        /* that's set by the OptionParser */
        void setValue(OptionValue value);
        OptionValue getValue() const;

        bool isValid() const;
        std::string getPlaceholder() const;

    private:
        std::string m_longName;
        std::string m_description;
        char        m_letter;
        OptionType  m_type;
        OptionValue m_value;
};


class OptionParser {
    public:
        void addOption(Option option);
        void addOption(const std::string &name, char letter,
                OptionType type = OT_FLAG,
                const std::string &description = "");

        void printHelp(std::ostream &os, const std::string &name) const;
        bool parse(int argc, char *argv[]);
        OptionValue getValue(const std::string &name);
        std::vector<std::string> getArgs();

    protected:
        Option &findOption(char letter);

    private:
        std::vector<Option> m_options;
        std::vector<std::string> m_args;
};

#endif /* OPTIONPARSER_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
