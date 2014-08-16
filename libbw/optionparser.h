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
#ifndef LIBBW_OPTIONPARSER_H_
#define LIBBW_OPTIONPARSER_H_

/**
 * \defgroup optparse  Option Parsing
 * \brief Generic option parser implementation
 *
 * This module contains a generic implementation of a command line parser.
 *
 * The parser itself only works on Unix since it users getopt_long()
 * internally.  However, libbw provides an own copy of getopt_long() which is
 * linked into libbw, so you don't notice that.
 *
 * See the documentation of the main class OptionParser in that file for an
 * example how to use the OptionParser. The API is influenced a bit by the
 * Python \c optparse package, but doesn't contain all functionality and also
 * needs a union type like OptionValue because C++ is not Python.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <list>
#include <map>
#include <string>
#include <vector>

namespace bw {

/* OptionType {{{ */

/**
 * \brief Option type
 *
 * This enumeration represents the type of an option.
 *
 * \ingroup optparse
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
 * \class OptionValue optionparser.h libbw/optionparser.h
 * \brief Value of a option
 *
 * This is a union-type that represents the value of a option. It's used as
 * return value in OptionParser.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup optparse
 */
class OptionValue {

public:
    /**
     * \brief Constructor
     *
     * Creates a new instance of a OptionValue with the type OptionType::OT_INVALID.
     */
    OptionValue();

public:
    /**
     * \brief Sets the type of an option.
     *
     * Sets the option type. See OptionType for a list of all valid types.
     *
     * \param[in] type the type
     */
    void setType(OptionType type);

    /**
     * \brief Returns the option type.
     *
     * Returns the type of an option. See OptionType for a list of all
     * valid types.
     *
     * \return the type
     */
    OptionType getType() const;

    /**
     * \brief Sets the string
     *
     * Sets the string value of an option. Makes only sense if the type is
     * OptionType::OT_STRING.
     *
     * \param[in] string the string value
     */
    void setString(const std::string &string);

    /**
     * \brief Returns the string
     *
     * Returns the string value of an option. Makes only sense if the type
     * is OptionType::OT_STRING.
     *
     * \return the string value of an option
     */
    std::string getString() const;

    /**
     * \brief Sets the flag value
     *
     * If type is OptionType::OT_FLAG, sets the value, i.e. \c true or \c
     * false.
     *
     * \param[in] flag the flag value
     */
    void setFlag(bool flag);

    /**
     * \brief Returns the flat value
     *
     * If type is OptionType::OT_STRING, returns the value, i.e. \c true
     * or \c false.
     *
     * \return the boolean value
     */
    bool getFlag() const;

    /**
     * \brief Sets the integer value
     *
     * If type is OptionType::OT_INTEGER, sets the value.
     *
     * \param[in] value the integer value
     */
    void setInteger(int value);

    /**
     * \brief Returns the integer value
     *
     * If type is OptionType::OT_INTEGER, returns the value.
     *
     * \return the integer value
     */
    int getInteger() const;

    /**
     * \brief Checks if the type contains valid data
     *
     * \return \c true if getValue() returns not OptionType::OT_INVALID.
     */
    operator bool() const;

private:
    OptionType      m_type;
    int             m_integer;
    std::string     m_string;
    bool            m_flag;
};

/* }}} */
/* Option {{{ */

/**
 * \class Option optionparser.h libbw/optionparser.h
 * \brief Option in an option parser
 *
 * This class represents a single option. A option has:
 *
 *  - a \b name which is used on the command line with <tt>--name</tt>
 *  - a short \b letter which is used on the command line with <tt>-l</tt>
 *  - a option \b type (OptionType::OT_FLAG, OptionType::OT_INTEGER or
 *    OptionType::OT_STRING)
 *  - a optional \b description which is needed in the help output
 *
 * See the description of OptionParser to get an idea how to use an Option to
 * parse arguments.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup optparse
 */
class Option {

public:
    /**
     * \brief Default constructor
     *
     * Creates a new option with type OT_FLAG. You can use setLongName(),
     * setLetter(), setType() and setDescription() to modify the elements
     * after construction.
     */
    Option();

    /**
     * \brief Rich constructor
     *
     * Creates a new instance of an Option. This Constructor can
     * initialise all properties.
     *
     * \param[in] name the long name of an option
     * \param[in] letter the short name of an option
     * \param[in] type the option type (OptionType::OT_FLAG, OptionType::OT_INTEGER
     *            or OptionType::OT_STRING)
     * \param[in] description a description for the help
     */
    Option(const std::string    &name,
           char                 letter,
           OptionType           type = OT_FLAG,
           const std::string    &description = "");

public:
    /**
     * \brief Sets the long name
     *
     * Sets the long name which is used like <tt>--long_name</tt> on the
     * command line.
     *
     * \param[in] name the name
     */
    void setLongName(const std::string &name);

    /**
     * \brief Returns the long name.
     *
     * Returns the long name which is used like <tt>--long_name</tt> on
     * the command line.
     *
     * \return the long name
     */
    std::string getLongName() const;

    /**
     * \brief Set the short letter
     *
     * Sets the short letter of the option which is used like <tt>-l</tt>
     * on the command line.
     *
     * \param[in] letter the letter of the option
     */
    void setLetter(char letter);

    /**
     * \brief Returns the letter
     *
     * Returns the short letter of the option which is used like
     * <tt>-l</tt> on the command line.
     *
     * \return the letter
     */
    char getLetter() const;

    /**
     * \brief Set the option type
     *
     * Sets the type of the option. Valid types are OptionType::OT_STRING
     * for string options, OptionType::OT_INTEGER for numeric options and
     * OptionType::OT_FLAG for boolean options.
     *
     * \param[in] type the option type
     */
    void setType(OptionType type);

    /**
     * \brief Return the option type
     *
     * Returns the type of the option. Valid types are OptionType::OT_STRING
     * for string options, OptionType::OT_INTEGER for numeric options and
     * OptionType::OT_FLAG for boolean options.
     *
     * \return the option type
     */
    OptionType getType() const;

    /**
     * \brief Sets the description
     *
     * Sets the description of the option. The description is needed to
     * print the help.
     *
     * \param[in] description the option description
     */
    void setDescription(const std::string &description);

    /**
     * \brief Returns the description
     *
     * Returns the description of the option.
     *
     * \return the option description
     */
    std::string getDescription() const;

    /**
     * \brief Sets the option value
     *
     * This function sets the value of the option. It's set by the option
     * parser, not by the user.
     *
     * \param[in] value the value of the option. The type of the
     *            OptionValue can either be OptionType::OT_INVALID or the
     *            type of the option (i.e. the return value of getType())
     */
    void setValue(OptionValue value);

    /**
     * \brief Returns the option value
     *
     * Returns the value of the option. If the option has no value, a
     * value with type OptionValue::OT_INVALID is returned.
     *
     * \return the value
     */
    OptionValue getValue() const;

    /**
     * \brief Checks if the option is valid
     *
     * Checks if the option has a valid value.
     *
     * \return \c true if the option is valid, \c false otherwise
     */
    bool isValid() const;

    /**
     * \brief Returns a placeholder for the option value
     *
     * This function is needed by the OptionParser to format the help. In
     * case this option is of type OptionType::OT_STRING, the string <tt>"<STRING>"</tt>
     * is returned. In case it's of the type OptionType::OT_INTEGER, the
     * string <tt>"<NUMBER>"</tt> is returned. In any other case, the
     * empty string (<tt>""</tt>) is returned.
     *
     * \return the placeholder as described above
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
/* OptionGroup {{{ */

/**
 * \class OptionGroup optionparser.h libbw/optionparser.h
 * \brief Group of options with a common title
 *
 * A option group represents a number of options that belongs together, like "debugging options" or
 * "input configuration". The only task of the option group is to group the listing of options in
 * the help output which is generated by OptionParser::printHelp().
 *
 * The title doesn't have to be unique and can be empty. There's always one option group by default
 * whose name can be set in the constructor of OptionParser. This makes it possible to use the
 * option parser without caring about option groups at all.
 *
 * \author Bernhard Walle
 * \ingroup optparse
 */
class OptionGroup {

    friend class OptionParser;

public:
    /**
     * \brief Constructor
     *
     * Creates a new OptionGroup object with \p title.
     *
     * \param[in] title the title for the option group. Once set, this title cannot be changed.
     * \note The m_title member is non-const to provide an assignment operator.
     */
    OptionGroup(const std::string &title="");

    /**
     * \brief Destructor
     */
    virtual ~OptionGroup() {}

public:
    /**
     * \brief Returns the title that has been set in the constructor.
     *
     * \return the title of the option group
     */
    std::string getTitle() const;

    /**
     * \brief Returns the options as vector
     *
     * \return a reference to the internal representation of options. The vector cannot be
     *         changed. Use OptionGroup::addOption() instead.
     * \sa OptionGroup::size()
     */
    const std::vector<Option> &options() const;

protected:
    /**
     * \brief Returns the options as vector (non-const)
     *
     * The non-const version is only accessible for friends.
     *
     * \return a reference to the internal representation of options. The vector cannot be
     *         changed. Use OptionGroup::addOption() instead.
     * \sa OptionGroup::size()
     */
    std::vector<Option> &options();

public:
    /**
     * \brief Returns the number of options in this group
     *
     * Convenience function for OptionGroup::options().size().
     *
     * \return the number of options in this option group.
     */
    size_t size() const;

    /**
     * \brief Adds a option to the option group
     *
     * \param[in] option the option that should be added.
     */
    void addOption(const Option &option);

    /**
     * \brief Adds an option to the option group
     *
     * Convenience method that doesn't need to create an Option object.
     * See also Option::Option(const std::string &, char, OptionType, const std::string &).
     *
     * \param[in] name the option name, see Option::setLongName()
     * \param[in] letter the short letter, see Option::setLetter()
     * \param[in] type the option type, see Option::setType()
     * \param[in] description the option description, see Option::setDescription()
     */
    void addOption(const std::string    &name,
                   char                 letter,
                   OptionType           type = OT_FLAG,
                   const std::string    &description = "");

private:
    std::string m_title;
    std::vector<Option> m_options;
};

/* }}} */
/* OptionParser {{{ */

/**
 * \class OptionParser optionparser.h libbw/optionparser.h
 * \brief Command line argument parser
 *
 * Class to parse command line arguments. The API is inspired by Python (\c optparse package), but
 * modified for C++.
 *
 * The option parser can be used with or without \b grouping. Grouping has no effect on the parsing
 * logic (which means that an option can exist only once in the application even in different option
 * groups) but only to the help output.
 *
 * Simple Example (without using grouped options):
 *
 * \code
 * bw::OptionParser op("Program Options");
 * op.addOption("debug", 'D', bw::OT_FLAG,
 *              "Enable debugging output");
 * op.addOption("help",  'h', bw::OT_FLAG,
 *              "Shows this help output");
 * op.addOption("label", 'l', bw::OT_STRING,
 *              "Boot the specified label without prompting");
 *
 * // do the parsing
 * bool ret = op.parse(argc, argv);
 * if (!ret)
 *     return false;
 *
 * // evaluate options
 * if (op.getValue("help").getFlag()) {
 *     op.printHelp(std::cerr, "program 0.1.0");
 *     return false;
 * }
 *
 * if (op.getValue("debug").getFlag())
 *     m_debug = true;
 * if (op.getValue("label"))
 *     m_label = op.getValue("label").getString();
 *
 * std::vector<std::string> args = op.getArgs();
 * if (args.size() > 1)
 *     return false;
 * if (args.size() == 1)
 *      m_argument = args[0];
 * \endcode
 *
 * This parsing code generates following help output:
<pre>
program 0.1.0

Program options:
    --debug | -D
        Enable debugging output
    --help | -h
        Shows this help output
    --label=&lt;STRING&gt; | -l &lt;STRING&gt;
        Boot the specified label without prompting
</pre>
 *
 * Advanced example (with using grouped options):
 * 
 * \code
 * int main(int argc, char *argv[])
 * {
 *      bw::OptionGroup generalGroup("General options");
 *      generalGroup.addOption("help", 'h', bw::OT_FLAG, "Shows this help output");
 *      generalGroup.addOption("version", 'v', bw::OT_FLAG, "Prints the program version");
 *      
 *      bw::OptionGroup debugGroup("Debug options");
 *      debugGroup.addOption("debug", 'D', bw::OT_FLAG, "Enable debugging output");
 *      debugGroup.addOption("debug-file", 'd', bw::OT_STRING, "Redirect debugging output to a file");
 *      
 *      bw::OptionParser op;
 *      op.addOptions(generalGroup);
 *      op.addOptions(debugGroup);
 *      
 *      // do the parsing
 *      bool ret = op.parse(argc, argv);
 *      if (!ret)
 *          return EXIT_FAILURE;
 *      
 *      //
 *      // general options
 *      //
 *      
 *      // help
 *      if (op.getValue("help").getFlag()) {
 *          op.printHelp(std::cerr, PACKAGE_STRING + " " + PACKAGE_VERSION);
 *          return EXIT_SUCCESS;
 *      }
 *      
 *      // version
 *      if (op.getValue("version"))
 *          std::cout << PACKAGE_STRING + " " + PACKAGE_VERSION << std::endl;
 *          return EXIT_SUCCESS;
 *      }
 *      
 *      //
 *      // debug options
 *      //
 *      
 *      // debug
 *      if (op.getValue("debug").getFlag())
 *          std::cout << "Debug enabled." << std::endl;
 *      
 *      // debug-file
 *      if (op.getValue("debug-file"))
 *          std::cout << "Redirection of debug output to file: "
 *                    << op.getValue("debug-file").getString() << std::endl;
 *      
 *      std::vector<std::string> args = op.getArgs();
 *      
 *      if (!args.empty()) {
 *          std::cout << "Arguments: " << std::endl;
 *          for (int i = 0; i < args.size(); ++i)
 *              std::cout << i << ":\t" << args[i] << std::endl;
 *      }
 *      
 *      return EXIT_SUCCESS;
 * }
 * \endcode
 *
 * And this one would generate following help output:
 *
<pre>
program 0.1.0

General options:
    --help | -h
        Shows this help output
    --version | -v
        Prints the program version

Debug options:
    --debug | -D
        Enable debugging output
    --debug-file=&lt;STRING&gt; | -d &lt;STRING&gt;
        Redirect debugging output to a file
</pre>
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup optparse
 */
class OptionParser {

public:
    /**
     * \brief Constructor
     *
     * \param[in] defaultGroupName the name of the default option group
     */
    OptionParser(const std::string &defaultGroupName="");

    /**
     * \brief Destructor
     */
    virtual ~OptionParser() {}

public:
    /**
     * \brief Adds an option
     *
     * Adds an option to default anonymous group of the OptionParser.
     *
     * This is a convenience function for simple option parsing. Code
     *
     * \code
     * bw::OptionGroup anonymouseGroup("");
     * anonymouseGroup.addOption("bla", 'b', bw::OT_FLAG, "Some description");
     * anonymouseGroup.addOption("fasel", 'f', bw::OT_FLAG, "Some other description");
     *
     * bw::OptionParser parser;
     * parser.addOptions(anonymouseGroup);
     * \endcode
     *
     * can be replaced with
     *
     * \code
     * bw::OptionParser parser;
     * parser.addOption("bla", 'b', bw::OT_FLAG, "Some description");
     * parser.addOption("fasel", 'f', bw::OT_FLAG, "Some other description");
     * \endcode
     *
     * \param[in] option the option to add
     */
    void addOption(Option option);

    /**
     * \brief Adds an option to the anonymouse grup
     *
     * Convenience method that doesn't need to create an Option object.
     * See also Option::Option(const std::string &, char, OptionType, const std::string &).
     *
     * This is a convenience function for simple option parsing. Code
     *
     * \code
     * bw::OptionGroup anonymouseGroup("");
     * anonymouseGroup.addOption(bw::Option("bla", 'b', bw::OT_FLAG, "Some description"));
     * anonymouseGroup.addOption(bw::Option("fasel", 'f', bw::OT_FLAG, "Some other description"));
     *
     * bw::OptionParser parser;
     * parser.addOptions(anonymouseGroup);
     * \endcode
     *
     * can be replaced with
     *
     * \code
     * bw::OptionParser parser;
     * parser.addOption(bw::Option("bla", 'b', bw::OT_FLAG, "Some description"));
     * parser.addOption(bw::Option("fasel", 'f', bw::OT_FLAG, "Some other description"));
     * \endcode
     *
     * \param[in] name the option name, see Option::setLongName()
     * \param[in] letter the short letter, see Option::setLetter()
     * \param[in] type the option type, see Option::setType()
     * \param[in] description the option description, see Option::setDescription()
     */
    void addOption(const std::string    &name,
                   char                 letter,
                   OptionType           type = OT_FLAG,
                   const std::string    &description = "");

    /**
     * \brief Adds a group of options to the option list
     *
     * The group appears as grouped list with a title in the help output.
     *
     * \param[in] group the group of options to add
     */
    void addOptions(const OptionGroup &group);

    /**
     * \brief Prints the help
     *
     * Auto-generates a help from the list of options and prints it to the
     * stream specified with\p os. The \p name is displayed on the
     * beginning and should contain the program name (and possibly a short
     * synopsis).
     *
     * Example:
     *
     * \code
     * op.printHelp(std::cerr, "myprogram [options] <filename>");
     * \endcode
     *
     * \param[in] os the output stream (e.g. std::cerr)
     * \param[in] name the program name, possibly with version and
     *            synopsis
     */
    void printHelp(std::ostream &os, const std::string &name) const;

    /**
     * \brief Parses the command line arguments
     *
     * Parses the command line arguments from main(). Both \p argc and \p
     * argv are not modified by parse().
     *
     * \param[in] argc the number of arguments
     * \param[in] argv the argument vector, being the program name the
     *            0-th element (i.e. ignored)
     * \return \c true if the command line has been parsed successfully,
     *         \c false if the command line did contain options that are invalid
     */
    bool parse(int argc, char *argv[]);

    /**
     * \brief Returns the option value for a option
     *
     * For option \p name (this is the long name), returns the option
     * value argument. If the option value did not occur in the command
     * line parsed with parse(), the option value with OptionType::OT_INVALID
     * is returned.
     *
     * \param[in] name the name of the parameter for which the value
     *            should be returned.
     * \return the option value
     */
    OptionValue getValue(const std::string &name);

    /**
     * \brief Returns the arguments
     *
     * Returns the arguments (not options). For example if you parse
     *
     * <tt>./bla --fasel=5 --debug filename</tt>
     *
     * then <tt>fasel</tt> and <tt>debug</tt> are \e options while
     * <tt>filename</tt> is the (only) \e argument.
     *
     * \return the argument vector, an empty vector if no arguments have
     *         been supplied by parse()
     */
    std::vector<std::string> getArgs();

protected:
    /**
     * \brief Finds a option
     *
     * Finds the option with the letter \p letter.
     *
     * \param[in] letter the short letter that characterizes the option
     * \return a reference of the option in m_options
     */
    Option &findOption(char letter);

    /**
     * \brief Calculates the total number of options
     *
     * The total number of options is the sum of options over all option groups.
     *
     * \return the total number of options.
     */
    int calcTotalNumberOfOptions() const;

private:
    std::vector<OptionGroup> m_options;
    std::vector<std::string> m_args;
};

/* }}} */

} // end namespace bw

#endif /* LIBBW_OPTIONPARSER_H_ */

// vim: set sw=4 ts=4 et fdm=marker:
