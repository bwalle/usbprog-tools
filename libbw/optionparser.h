/* {{{
 * Copyright (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

/**
 * @file
 * @brief Generic option parser implementation
 *
 * This file contains a generic implementation of a command line parser.
 *
 * The parser itself only works on Unix since it users getopt_long()
 * internally.  However, libbw provides an own copy of getopt_long() which is
 * linked into libbw, so you don't notice that.
 *
 * See the documentation of the main class OptionParser in that file for an
 * example how to use the OptionParser. The API is influenced a bit by the
 * Python @c optparse package, but doesn't contain all functionality and also
 * needs a union type like OptionValue because C++ is not Python.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */

#include <list>
#include <map>
#include <string>
#include <vector>

namespace bw {

/* OptionType {{{ */

/**
 * @brief Option type
 *
 * This enumeration represents the type of an option.
 */
enum OptionType {
    OT_INVALID,         /**< invalid option type */
    OT_STRING,          /**< string option */
    OT_INTEGER,         /**< integer option */
    OT_FLAG             /**< boolean option (without argument, just present or not present */
};

/* }}} */
/* OptionValue {{{ */

/**
 * @class OptionValue optionparser.h libbw/optionparser.h
 * @brief Value of a option
 *
 * This is a union-type that represents the value of a option. It's used as
 * return value in OptionParser.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class OptionValue {

    public:
        /**
         * @brief Constructor
         *
         * Creates a new instance of a OptionValue with the type OptionType::OT_INVALID.
         */
        OptionValue();

    public:
        /**
         * @brief Sets the type of an option.
         *
         * Sets the option type. See OptionType for a list of all valid types.
         *
         * @param[in] type the type
         */
        void setType(OptionType type);

        /**
         * @brief Returns the option type.
         *
         * Returns the type of an option. See OptionType for a list of all
         * valid types.
         *
         * @return the type
         */
        OptionType getType() const;

        /**
         * @brief Sets the string
         *
         * Sets the string value of an option. Makes only sense if the type is
         * OptionType::OT_STRING.
         *
         * @param[in] string the string value
         */
        void setString(const std::string &string);

        /**
         * @brief Returns the string
         *
         * Returns the string value of an option. Makes only sense if the type
         * is OptionType::OT_STRING.
         *
         * @return the string value of an option
         */
        std::string getString() const;

        /**
         * @brief Sets the flag value
         *
         * If type is OptionType::OT_FLAG, sets the value, i.e. @c true or @c
         * false.
         *
         * @param[in] flag the flag value
         */
        void setFlag(bool flag);

        /**
         * @brief Returns the flat value
         *
         * If type is OptionType::OT_STRING, returns the value, i.e. @c true
         * or @c false.
         *
         * @return the boolean value
         */
        bool getFlag() const;

        /**
         * @brief Sets the integer value
         *
         * If type is OptionType::OT_INTEGER, sets the value.
         *
         * @param[in] value the integer value
         */
        void setInteger(int value);

        /**
         * @brief Returns the integer value
         *
         * If type is OptionType::OT_INTEGER, returns the value.
         *
         * @return the integer value
         */
        int getInteger() const;

    private:
        OptionType      m_type;
        int             m_integer;
        std::string     m_string;
        bool            m_flag;
};

/* }}} */
/* Option {{{ */

/**
 * @class Option optionparser.h libbw/optionparser.h
 * @brief Option in an option parser
 *
 * This class represents a single option. A option has:
 *
 *  - a @b name which is used on the command line with <tt>--name</tt>
 *  - a short @b letter which is used on the command line with <tt>-l</tt>
 *  - a option @b type (OptionType::OT_FLAG, OptionType::OT_INTEGER or
 *    OptionType::OT_STRING)
 *  - a optional @b description which is needed in the help output
 *
 * See the description of OptionParser to get an idea how to use an Option to
 * parse arguments.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Option {
    public:
        /**
         * @brief Default constructor
         *
         * Creates a new option with type OT_FLAG. You can use setLongName(),
         * setLetter(), setType() and setDescription() to modify the elements
         * after construction.
         */
        Option();

        /**
         * @brief Rich constructor
         *
         * Creates a new instance of an Option. This Constructor can
         * initialise all properties.
         *
         * @param[in] name the long name of an option
         * @param[in] letter the short name of an option
         * @param[in] type the option type (OptionType::OT_FLAG, OptionType::OT_INTEGER
         *            or OptionType::OT_STRING)
         * @param[in] description a description for the help
         */
        Option(const std::string &name, char letter,
                OptionType type = OT_FLAG,
                const std::string &description = "");

    public:
        /**
         * @brief Sets the long name
         *
         * Sets the long name which is used like <tt>--long_name</tt> on the
         * command line.
         *
         * @param[in] name the name
         */
        void setLongName(const std::string &name);

        /**
         * @brief Returns the long name.
         *
         * Returns the long name which is used like <tt>--long_name</tt> on
         * the command line.
         *
         * @return the long name
         */
        std::string getLongName() const;

        /**
         * @brief Set the short letter
         *
         * Sets the short letter of the option which is used like <tt>-l</tt>
         * on the command line.
         *
         * @param[in] letter the letter of the option
         */
        void setLetter(char letter);

        /**
         * @brief Returns the letter
         *
         * Returns the short letter of the option which is used like
         * <tt>-l</tt> on the command line.
         *
         * @return the letter
         */
        char getLetter() const;

        /**
         * @brief Set the option type
         *
         * Sets the type of the option. Valid types are OptionType::OT_STRING
         * for string options, OptionType::OT_INTEGER for numeric options and
         * OptionType::OT_FLAG for boolean options.
         *
         * @param[in] type the option type
         */
        void setType(OptionType type);

        /**
         * @brief Return the option type
         *
         * Returns the type of the option. Valid types are OptionType::OT_STRING
         * for string options, OptionType::OT_INTEGER for numeric options and
         * OptionType::OT_FLAG for boolean options.
         *
         * @return the option type
         */
        OptionType getType() const;

        /**
         * @brief Sets the description
         *
         * Sets the description of the option. The description is needed to
         * print the help.
         *
         * @param[in] description the option description
         */
        void setDescription(const std::string &description);

        /**
         * @brief Returns the description
         *
         * Returns the description of the option.
         *
         * @return the option description
         */
        std::string getDescription() const;

        /**
         * @brief Sets the option value
         *
         * This function sets the value of the option. It's set by the option
         * parser, not by the user.
         *
         * @param[in] value the value of the option. The type of the
         *            OptionValue can either be OptionType::OT_INVALID or the
         *            type of the option (i.e. the return value of getType())
         */
        void setValue(OptionValue value);

        /**
         * @brief Returns the option value
         *
         * Returns the value of the option. If the option has no value, a
         * value with type OptionValue::OT_INVALID is returned.
         *
         * @return the value
         */
        OptionValue getValue() const;

        /**
         * @brief Checks if the option is valid
         *
         * Checks if the option has a valid value.
         *
         * @return @c true if the option is valid, @c false otherwise
         */
        bool isValid() const;

        /**
         * @brief Returns a placeholder for the option value
         *
         * This function is needed by the OptionParser to format the help. In
         * case this option is of type OptionType::OT_STRING, the string <tt>"<STRING>"</tt>
         * is returned. In case it's of the type OptionType::OT_INTEGER, the
         * string <tt>"<NUMBER>"</tt> is returned. In any other case, the
         * empty string (<tt>""</tt>) is returned.
         *
         * @return the placeholder as described above
         */
        std::string getPlaceholder() const;

    private:
        std::string m_longName;
        std::string m_description;
        char        m_letter;
        OptionType  m_type;
        OptionValue m_value;
};

/* }}} */
/* OptionParser {{{ */

/**
 * @class OptionParser optionparser.h libbw/optionparser.h
 * @brief Command line argument parser
 *
 * Class to parse command line arguments. The API is inspired by Python (@c
 * optparse package), but modified for C++.
 *
 * Example:
 *
 * @code
 * bw::OptionParser op;
 * op.addOption(Option("debug", 'D', bw::OT_FLAG, "Enable debugging output"));
 * op.addOption(Option("help", 'h', bw::OT_FLAG, "Shows this help output"));
 * op.addOption(Option("label", 'l', bw::OT_STRING, "Boot the specified label without prompting"));
 *
 * // do the parsing
 * bool ret = op.parse(argc, argv);
 * if (!ret)
 *     return false;
 *
 * // evaluate options
 * if (op.getValue("help").getFlag()) {
 *     op.printHelp(cerr, PACKAGE_STRING " " PACKAGE_VERSION);
 *     return false;
 * }
 *
 * if (op.getValue("debug").getFlag())
 *     m_debug = true;
 * if (op.getValue("label").getType() != bw::OT_INVALID)
 *     m_label = op.getValue("label").getString();
 *
 * std::vector<std::string> args = op.getArgs();
 * if (args.size() > 1)
 *     return false;
 * if (args.size() == 1)
 *      m_argument = args[0];
 * @endcode
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class OptionParser {
    public:
        /**
         * @brief Adds an option
         *
         * Adds an option to the OptionParser.
         *
         * @param[in] option the option to add
         */
        void addOption(Option option);

        /**
         * @brief Adds an option
         *
         * Convenience method that doesn't need to create an Option object.
         * See also Option::Option(const std::string &, char, OptionType, const std::string &).
         *
         * @param[in] name the option name, see Option::setLongName()
         * @param[in] letter the short letter, see Option::setLetter()
         * @param[in] type the option type, see Option::setType()
         * @param[in] description the option description, see Option::setDescription()
         */
        void addOption(const std::string &name, char letter,
                OptionType type = OT_FLAG,
                const std::string &description = "");

        /**
         * @brief Prints the help
         *
         * Auto-generates a help from the list of options and prints it to the
         * stream specified with@p os. The @p name is displayed on the
         * beginning and should contain the program name (and possibly a short
         * synopsis).
         *
         * Example:
         *
         * @code
         * op.printHelp(std::cerr, "myprogram [options] <filename>");
         * @endcode
         *
         * @param[in] os the output stream (e.g. std::cerr)
         * @param[in] name the program name, possibly with version and
         *            synopsis
         */
        void printHelp(std::ostream &os, const std::string &name) const;

        /**
         * @brief Parses the command line arguments
         *
         * Parses the command line arguments from main(). Both @p argc and @p
         * argv are not modified by parse().
         *
         * @param[in] argc the number of arguments
         * @param[in] argv the argument vector, being the program name the
         *            0-th element (i.e. ignored)
         * @return @c true if the command line has been parsed successfully,
         *         @c false if the command line did contain options that are invalid
         */
        bool parse(int argc, char *argv[]);

        /**
         * @brief Returns the option value for a option
         *
         * For option @p name (this is the long name), returns the option
         * value argument. If the option value did not occur in the command
         * line parsed with parse(), the option value with OptionType::OT_INVALID
         * is returned.
         *
         * @param[in] name the name of the parameter for which the value
         *            should be returned.
         * @return the option value
         */
        OptionValue getValue(const std::string &name);

        /**
         * @brief Returns the arguments
         *
         * Returns the arguments (not options). For example if you parse
         *
         * <tt>./bla --fasel=5 --debug filename</tt>
         *
         * then <tt>fasel</tt> and <tt>debug</tt> are @e options while
         * <tt>filename</tt> is the (only) @e argument.
         *
         * @return the argument vector, an empty vector if no arguments have
         *         been supplied by parse()
         */
        std::vector<std::string> getArgs();

    protected:
        /**
         * @brief Finds a option
         *
         * Finds the option with the letter @p letter.
         *
         * @param[in] letter the short letter that characterizes the option
         * @return a reference of the option in m_options
         */
        Option &findOption(char letter);

    private:
        std::vector<Option> m_options;
        std::vector<std::string> m_args;
};

/* }}} */

} // end namespace bw

#endif /* OPTIONPARSER_H */

// vim: set sw=4 ts=4 et fdm=marker:
