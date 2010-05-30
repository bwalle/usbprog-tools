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
 * @file date.h
 * @brief Platform-independent date/time functions for USBprog
 *
 * This file contains date/time functions that work on both POSIX and Win32 platforms.
 *
 * @author Bernhard Walle <bernhard@bwalle.de>
 * @ingroup core
 */

#ifndef USBPROG_DATE_H
#define USBPROG_DATE_H

#include <ctime>
#include <stdexcept>
#include <string>

#include <usbprog-core/error.h>

namespace usbprog {
namespace core {

/* DateTimeFormat {{{ */

/**
 * @brief Enumeration that describe different string representations of date/time.
 * @ingroup core
 */
enum DateTimeFormat {
    DTF_ISO_DATETIME           /**< ISO format for date and time (<tt>YYYY-MM-DD HH:MM</tt>) */,
    DTF_ISO_DATE               /**< ISO format for date without time (<tt>YYYY-MM-DD</tt>) */,
    DTF_ISO_SHORT_DATETIME     /**< short ISO format for date and time (<tt>YY-MM-DD HH:MM</tt>) */,
    DTF_ISO_SHORT_DATE         /**< short ISO format for date without time (<tt>YY-MM-DD</tt>) */
};

/* }}} */
/* DateTime {{{ */

/**
 * @brief Represents a specific point of time
 *
 * A object of this class represents a point of time, e.g. the 1st January 1970 at 20:30 o'clock.
 *
 * @ingroup core
 * @author Bernhard Walle <bernhard@bwalle.de>
 */
class DateTime {

    public:
        /**
         * @brief Default constructor
         *
         * Creates a date/time object which is set to now.
         */
        DateTime();

        /**
         * @brief Constructor to set the date/time object to a time_t value
         *
         * Initialises the created date/time object with @p time which is a standard C representation
         * in seconds since the epoch (1970-01-01 00:00 GMT).
         *
         * @param[in] time the @c time_t object as described above
         */
        DateTime(time_t time);

        /**
         * @brief Constructor to set the date/time object to a struct tm value
         *
         * Initialises the created date/time object with @p time which is a standard C representation
         * as split-out <tt>struct tm</tt>. Please read the documentation of <tt>mktime(3)</tt> to see
         * which members of @p time must be initialised.
         *
         * @param[in] time the <tt>struct tm</tt> pointer as described above
         */
        DateTime(struct tm *time);

        /**
         * @brief Constructor that sets the date/time object to the result of a string parsing
         *
         * This constructor parses @p string which must be in the given @p format and initialises
         * the date/time object with the result.
         *
         * @param[in] string the string that specifies the date/time, e.g. <tt>"1970-01-01"</tt>
         * @param[in] format the format specification for the string, e.g. DateTimeFormat::DTF_ISO_DATE
         *            which would be a correct format for the example string of @p string above
         * @exception ParseError if parsing @p string with @p format failed
         */
        DateTime(const std::string &string, DateTimeFormat format)
        throw (ParseError);

    public:
        /**
         * @brief Sets the date/time value of this object to @p time
         *
         * Sets the value to @p time which is a standard C representation in seconds since the epoch
         * (1970-01-01 00:00 GMT).
         *
         * @param[in] time the @c time_t object as described above
         */
        void setDateTime(time_t time);

        /**
         * @brief Sets the date/time value of this object to a struct tm value
         *
         * Sets the value of this date/time object to @p time which is a standard C representation
         * as split-out <tt>struct tm</tt>. Please read the documentation of <tt>mktime(3)</tt> to see
         * which members of @p time must be initialised.
         *
         * @param[in] time the <tt>struct tm</tt> pointer as described above
         */
        void setDateTime(struct tm *time);

        /**
         * @brief Sets the date/time object to the result of a string parsing
         *
         * This member function parses @p string which must be in the given @p format and initialises
         * the date/time object with the result.
         *
         * @param[in] string the string that specifies the date/time, e.g. <tt>"1970-01-01"</tt>
         * @param[in] format the format specification for the string, e.g. DateTimeFormat::DTF_ISO_DATE
         *            which would be a correct format for the example string of @p string above
         * @exception ParseError if parsing @p string with @p format failed
         */
        void setDateTime(const std::string &string, DateTimeFormat format)
        throw (ParseError);

        /**
         * @brief Returns the @c time_t representation of the time value
         *
         * Returns the seconds since the epoch of the current date/time value, regardless how it
         * has been set.
         *
         * @return the seconds since the epoch
         */
        time_t getDateTimeSeconds() const;

        /**
         * @brief Returns the <tt>struct tm</tt> representation of the time value
         *
         * Returns the split-out <tt>struct tm</tt> representation of the time. The definition
         * of <tt>struct tm</tt> according to the C standard is as follows:
         *
         * @code
         * struct tm {
         *     int tm_sec;         // seconds
         *     int tm_min;         // minutes
         *     int tm_hour;        // hours
         *     int tm_mday;        // day of the month
         *     int tm_mon;         // month
         *     int tm_year;        // year
         *     int tm_wday;        // day of the week
         *     int tm_yday;        // day in the year
         *     int tm_isdst;       // daylight saving time
         * };
         * @endcode
         *
         * @return the value as described above
         */
        struct tm getDateTimeTm() const;

        /**
         * @brief Formats the date/time as string
         *
         * Returns a string representation of the date/time value of this object. See also
         * the documentation DateTimeFormat.
         *
         * @param[in] format the format specification for the resulting string value
         * @return the string representation, e.g. <tt>1970-01-01 18:30</tt> if
         *         @p format is DateTimeFormat::DTF_ISO_DATETIME.
         */
        std::string getDateTimeString(DateTimeFormat format) const;

    private:
        time_t      m_dateTime;
};

/**
 * @brief Equal operator for DateTime objects
 *
 * Checks if @p a is equal to @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is equal to @p b
 * @ingroup core
 */
bool operator==(const DateTime &a, const DateTime &b);

/**
 * @brief Not equal operator for DateTime objects
 *
 * Checks if @p a is not equal to @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is not equal to @p b
 * @ingroup core
 */
bool operator!=(const DateTime &a, const DateTime &b);

/**
 * @brief Less equal operator for DateTime objects
 *
 * Checks if @p a is less or equal to @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is less or equal to @p b
 * @ingroup core
 */
bool operator<=(const DateTime &a, const DateTime &b);

/**
 * @brief Less then operator for DateTime objects
 *
 * Checks if @p a is less then @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is less then @p b
 * @ingroup core
 */
bool operator<(const DateTime &a, const DateTime &b);

/**
 * @brief Greater then operator for DateTime objects
 *
 * Checks if @p a is greater then @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is greater then @p b
 * @ingroup core
 */
bool operator>(const DateTime &a, const DateTime &b);

/**
 * @brief Greater equal operator for DateTime objects
 *
 * Checks if @p a is greater or equal to @p b.
 *
 * @param[in] a the first object to check
 * @param[in] b the second object to check
 * @return @c true if @p a is greater or equal to @p b
 * @ingroup core
 */
bool operator>=(const DateTime &a, const DateTime &b);

/**
 * @brief Minus operator for DateTime objects
 *
 * Subtracts @p b from @p a and returns the result as seconds.
 *
 * @param[in] a the minuend
 * @param[in] b the subtrahend
 * @return the difference in seconds
 * @ingroup core
 */
long long operator-(const DateTime &a, const DateTime &b);

/**
 * @brief Plus operator for DateTime objects
 *
 * Adds @p a to @p b and returns the result as seconds.
 *
 * @param[in] a the first summand
 * @param[in] b the second summand
 * @return the sum of @p a and @p b
 * @ingroup core
 */
long long operator+(const DateTime &a, const DateTime &b);

/* }}} */

} // end namespace core
} // end namespace usbprog

#endif /* USBPROG_DATE_H */

// vim: set sw=4 ts=4 et: :collapseFolds=1:
