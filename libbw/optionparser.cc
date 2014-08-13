/* {{{
 * Copyright (c) 2007-2011, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "bwconfig.h"

#define _GNU_SOURCE 1
#include <getopt.h>
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#include <string.h>

#include "optionparser.h"

namespace bw {

/* OptionValue {{{ */

OptionValue::OptionValue()
    : m_type(OT_INVALID)
    , m_integer(0)
    , m_string("")
    , m_flag(false)
{}

void OptionValue::setType(OptionType type)
{
    m_type = type;
}

OptionType OptionValue::getType() const
{
    return m_type;
}

void OptionValue::setString(const std::string &string)
{
    m_string = string;
}

std::string OptionValue::getString() const
{
    return m_string;
}

void OptionValue::setFlag(bool flag)
{
    m_flag = flag;
}

bool OptionValue::getFlag() const
{
    return m_flag;
}

void OptionValue::setInteger(int value)
{
    m_integer = value;
}

int OptionValue::getInteger() const
{
    return m_integer;
}

OptionValue::operator bool() const
{
    return m_type != OT_INVALID;
}

/* }}} */
/* Option {{{ */

Option::Option()
    : m_type(OT_FLAG)
{}

Option::Option(const std::string &name, char letter, OptionType type,
               const std::string &description)
    : m_longName(name)
    , m_description(description)
    , m_letter(letter)
    , m_type(type)
{}

void Option::setLongName(const std::string &name)
{
    m_longName = name;
}

std::string Option::getLongName() const
{
    return m_longName;
}

void Option::setLetter(char letter)
{
    m_letter = letter;
}

char Option::getLetter() const
{
    return m_letter;
}

void Option::setType(OptionType type)
{
    m_type = type;
}

OptionType Option::getType() const
{
    return m_type;
}

void Option::setDescription(const std::string &description)
{
    m_description = description;
}

std::string Option::getDescription() const
{
    return m_description;
}

void Option::setValue(OptionValue value)
{
    m_value = value;
}

OptionValue Option::getValue() const
{
    return m_value;
}

bool Option::isValid() const
{
    return m_longName.size() > 0 && m_letter != 0;
}

std::string Option::getPlaceholder() const
{
    switch (getType()) {
        case OT_STRING:
            return "<STRING>";

        case OT_INTEGER:
            return "<NUMBER>";

        default:
            return "";
    }
}

/* }}} */
/* OptionGroup {{{ */

OptionGroup::OptionGroup(const std::string &title)
    : m_title(title)
{}

std::string OptionGroup::getTitle() const
{
    return m_title;
}

const std::vector<Option> &OptionGroup::options() const
{
    return m_options;
}

std::vector<Option> &OptionGroup::options()
{
    return m_options;
}

size_t OptionGroup::size() const
{
    return m_options.size();
}

void OptionGroup::addOption(const Option &option)
{
    m_options.push_back(option);
}

void OptionGroup::addOption(const std::string    &name,
                            char                 letter,
                            OptionType           type,
                            const std::string    &description)
{
    addOption(Option(name, letter, type, description));
}

/* }}} */
/* OptionParser {{{ */

OptionParser::OptionParser(const std::string &defaultGroupName)
{
    m_options.push_back(OptionGroup(defaultGroupName));
}

void OptionParser::addOption(Option option)
{
    m_options[0].addOption(option);
}

void OptionParser::addOption(const std::string &name, char letter,
        OptionType type, const std::string &description)
{
    Option op(name, letter, type, description);
    addOption(op);
}

void OptionParser::addOptions(const OptionGroup &group)
{
    m_options.push_back(group);
}

bool OptionParser::parse(int argc, char *argv[])
{
    struct option *cur, *opt;
    std::string   getopt_string;
    int totalNumber = calcTotalNumberOfOptions();

    opt = new option[totalNumber + 1];
    if (!opt) {
        std::cerr << "OptionParser::parse(): malloc failed" << std::endl;
        return false;
    }
    cur = opt;

    for (std::vector<OptionGroup>::const_iterator it = m_options.begin();
            it != m_options.end(); ++it) {

        const std::vector<Option> options = it->options();

        for (std::vector<Option>::const_iterator opIter = options.begin();
                opIter != options.end(); ++opIter) {

            const Option opt = *opIter;
            cur->name = strdup(opt.getLongName().c_str());
            cur->has_arg = opt.getType() != OT_FLAG;
            cur->flag = 0;
            cur->val = opt.getLetter();

            getopt_string += opt.getLetter();
            if (opt.getType() != OT_FLAG)
                getopt_string += ":";

            cur++;
        }
    }
    memset(cur, 0, sizeof(option));

    // now parse the options
    int c;

    for (;;) {
        int option_index = 0;

        c = getopt_long(argc, argv, getopt_string.c_str(),
                opt, &option_index);
        if (c == -1)
            break;

        Option &current_option = findOption(c);
        if (!current_option.isValid()) {
            std::cerr << "Invalid option: " << (char)c << std::endl;
            break;
        }

        OptionValue v;
        v.setType(current_option.getType());
        switch (current_option.getType()) {
            case OT_FLAG:
                v.setFlag(true);
                current_option.setValue(v);
                break;

            case OT_INTEGER:
                v.setInteger(std::atoi(optarg));
                break;

            case OT_STRING:
                v.setString(std::string(optarg));
                break;

            default:
                break;

        }
        current_option.setValue(v);

    }

    // save arguments
    if (optind < argc)
        while (optind < argc)
            m_args.push_back(argv[optind++]);

    // free stuff
    cur = opt;
    for (int i = 0; i < totalNumber; i++) {
        free((void *)cur->name);
        cur++;
    }
    delete[] opt;

    return true;
}

OptionValue OptionParser::getValue(const std::string &name)
{
    for (std::vector<OptionGroup>::const_iterator it = m_options.begin();
            it != m_options.end(); ++it) {

        const std::vector<Option> options = it->options();

        for (std::vector<Option>::const_iterator opIter = options.begin();
                opIter != options.end(); ++opIter) {

            const Option &op = *opIter;

            if (op.getLongName() == name)
                return op.getValue();
        }
    }

    return OptionValue();
}

Option &OptionParser::findOption(char letter)
{
    static Option invalid;

    for (std::vector<OptionGroup>::iterator it = m_options.begin();
            it != m_options.end(); ++it) {

        std::vector<Option> &options = it->options();

        for (std::vector<Option>::iterator opIter = options.begin();
                opIter != options.end(); ++opIter) {

            Option &opt = *opIter;

            if (opt.getLetter() == letter)
                return opt;
        }
    }

    return invalid;
}

std::vector<std::string> OptionParser::getArgs()
{
    return m_args;
}

void OptionParser::printHelp(std::ostream &os, const std::string &name) const
{
    os << name << std::endl << std::endl;

    bool firstGroup = true;
    for (std::vector<OptionGroup>::const_iterator it = m_options.begin();
            it != m_options.end(); ++it) {

        const OptionGroup &group = *it;

        // skip empty option groups
        if (group.size() == 0)
            continue;

        // if it's not the first group, print a bit of vertical space
        if (firstGroup)
            firstGroup = false;
        else
            os << std::endl;

        // print the title only if there's a title
        if (!group.getTitle().empty())
             os << group.getTitle() << ":" << std::endl;

        const std::vector<Option> options = it->options();

        for (std::vector<Option>::const_iterator opIter = options.begin();
                opIter != options.end(); ++opIter) {

            const Option &opt = *opIter;

            os << "    --" << opt.getLongName();
            std::string placeholder = opt.getPlaceholder();
            if (placeholder.length() > 0)
                os << "=" << opt.getPlaceholder();
            os << " | -" << opt.getLetter();
            if (placeholder.length() > 0)
                os << " " << opt.getPlaceholder();
            os << std::endl;
            os << "        " << opt.getDescription() << std::endl;
        }
    }
}

int OptionParser::calcTotalNumberOfOptions() const
{
    int total = 0;

    for (std::vector<OptionGroup>::const_iterator it = m_options.begin();
            it != m_options.end(); ++it)
        total += it->size();

    return total;
}

/* }}} */

} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
