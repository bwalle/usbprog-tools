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
 * @file util.h
 * @brief Platform-independent functions for USBprog from various areas
 *
 * This file contains various platform-independent (which means that it works for both POSIX
 * and Win32) abstractions, mostly in the file handling area.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef USBPROG_UTIL_H
#define USBPROG_UTIL_H

#include <usbprog-core/date.h>
#include <usbprog-core/types.h>

namespace usbprog {
namespace core {

/* Fileutil {{{ */

/**
 * @brief Path handling functions
 *
 * This class contains only static functions, used for file and path handling.
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class Fileutil {
public:
    /**
     * @brief Returns the home directory
     *
     * On Unix, that is the entry in the <tt>/etc/fstab</tt> file which is mostly <tt>/home/<i>USERNAME</i></tt>
     * on Linux or <tt>/Users/<i>USERNAME</i></tt> on Mac OS. On Windows, it's in most cases
     * <tt>c:\\Documents and Settings\\<i>USERNAME</i></tt>, but it depends on the language.
     *
     * @return the home directory as described above.
     */
    static std::string homeDir();

    /**
     * @brief Returns the configuration directory
     *
     * Returns a directory name that is suitable for storing the configuration of program
     * @p program. On POSIX systems, that's <tt>$HOME/.<i>PROGRAMNAME</i></tt> and on Windows, it's
     * <tt>c:\\Documents and Settings\\<i>USERNAME</i>\\Application Data</tt>.
     *
     * @param[in] program the name of the program as described above
     * @return the configuration directory as described above.
     */
    static std::string configDir(const std::string &program);

    /**
     * @brief Creates the directory @p dir
     *
     * This function creates the directory @p dir. It's necessary that the parent directory exists,
     * so this function doesn't create the directories recursively.
     *
     * @param[in] dir the directory to create
     * @return @c true if the creation of the directory was successful, @c false otherwise.
     */
    static bool mkdir(const std::string &dir);

    /**
     * @brief Checks if @p dir is a directory
     *
     * Checks if @p dir is a directory (and not an ordinary file).
     *
     * @param[in] dir the directory name to check for
     * @return @c true if @p dir exists and is a directory, @c false otherwise
     */
    static bool isDir(const std::string &dir);

    /**
     * @brief Checks if @p file is a file
     *
     * Checks if @p file is a regular file, i.e. not a directory.
     *
     * @param[in] file the file name to check for
     * @return @c true if @p file exists and is a regular file, @c false otherwise
     */
    static bool isFile(const std::string &file);

    /**
     * @brief Checks if @p path is a path name
     *
     * Checks if @p path is a path name. This function is used by USBprog to check if
     * the user specified a path name as firmware or a firmware name of the database.
     * The semantics of the function is not very well definied.
     *
     * @param[in] path the path to check for
     * @return @c true if @p path looks like a path name, @c false otherwise.
     */
    static bool isPathName(const std::string &path);

    /**
     * @brief Resolve the path @p path
     *
     * Implements the shell meta-character functionality for <tt>'~'</tt> and <tt>'~<i>username</i></tt>.
     * This works only on POSIX. On Win32, this function does nothing than returning @p path.
     *
     * @param[in] path the path name to resolve
     * @return the resolved path as described above
     */
    static std::string resolvePath(const std::string &path);

    /**
     * @brief Returns the modification time of @p file
     *
     * @param[in] file the name of the file for which the modification time should be retrieved.
     * @return the modification time of @p file as DateTime object
     * @exception IOError if @p file doesn't exist or cannot be opened to read the meta-data.
     */
    static DateTime getMTime(const std::string &file);

    /**
     * @brief Reads the bytes from @p file
     *
     * @param[in] file the name of the file that should be read
     * @return the contents of @p file as ByteVector
     * @exception IOError if the file cannot be read, i.e. if it doesn't exist.
     */
    static ByteVector readBytesFromFile(const std::string &file);
};

/* }}} */
/* Free functions {{{ */

/**
 * @brief Concatenates two path components
 *
 * Platform-independent concatenate function for @p a and @p b. For example
 *
 * @code
 * std::string home_bin = pathconcat(Fileutil::homeDir(), "bin");
 * @endcode
 *
 * will return <tt>'/home/<i>USERNAME</i>/bin'</tt> on POSIX.
 *
 * @param[in] a the first path component to concat
 * @param[in] b the second path component to concat
 * @return the concatenated path components.
 * @ingroup core
 */
std::string pathconcat(const std::string &a, const std::string &b);

/**
 * @brief Sleeps for @p seconds seconds
 *
 * @param[in] seconds the sleep value in seconds
 * @see usbprog_msleep()
 * @ingroup core
 */
void usbprog_sleep(unsigned int seconds);

/**
 * @brief Sleeps for @p msec milliseconds
 *
 * @param[in] msec the sleep value in milliseconds
 * @see usbprog_sleep()
 * @ingroup core
 */
void usbprog_msleep(unsigned int msec);

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_UTIL_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
