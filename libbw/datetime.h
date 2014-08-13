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
#ifndef DATETIME_H
#define DATETIME_H

#include <iostream>
#include <string>
#include <ctime>

#include "compiler.h"

namespace bw {

/* Datetime {{{ */

/**
 * \class Datetime datetime.h libbw/datetime.h
 *
 * This class represents an absolute point of time. Therefore, no <tt>+</tt> or <tt>-</tt> operators
 * are provided because it doesn't make sense to add two absolute time points.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup datetime
 */
class Datetime {

    public:
        /**
         * \brief Symbolic month names
         */
        enum MonthNames {
            January   = 1,
            February  = 2,
            March     = 3,
            April     = 4,
            May       = 5,
            June      = 6,
            July      = 7,
            August    = 8,
            September = 9,
            October   = 10,
            November  = 11,
            December  = 12
        };

        /**
         * @brief Enumeration for the weekday
         */
        enum Weekday {
            Monday      = 1,//!< Monday
            Tuesday     = 2,//!< Tuesday
            Wednesday   = 3,//!< Wednesday
            Thursday    = 4,//!< Thursday
            Friday      = 5,//!< Friday
            Saturday    = 6,//!< Saturday
            Sunday      = 7 //!< Sunday
        };

    public:
        /**
         * \brief Default constructor
         *
         * Creates an invalid datetime object with a timestamp of 0.
         */
        Datetime();

        /**
         * \brief Creates a new Datetime object from a Unix time
         *
         * \param[in] time seconds since the epoch
         */
        explicit Datetime(const time_t &time);

        /**
         * \brief Creates a new Datetime object from broken-up time
         *
         * The UTC flag determines if the specified date is UTC. It doesn't affect the functionality
         * useUtc() and setUseUtc() which determine the result of the query function.
         *
         * \param[in] year the year, e.g. 2011
         * \param[in] month the month from 1 to 12. Values from enum MonthNames may be used.
         * \param[in] day the day of the month, e.g. 1 or 31.
         * \param[in] hour the hour from 0 to 23.
         * \param[in] minute the minute from 0 to 59.
         * \param[in] second the second from 0 to 60 (60 means leap second).
         * \param[in] utc \c true if the broken-up time is UTC, \c false if it's localtime.
         */
        Datetime(int year, int month, int day, int hour, int minute, int second, bool utc);

        /**
         * \brief Destructor
         */
        virtual ~Datetime() {}

    public:
        /**
         * \brief Returns a Datetime object with the current time set
         *
         * \return the newly created datetime object.
         */
        static Datetime now();

    public:
        /**
         * \brief Returns the timestamp
         *
         * \return the timestamp in seconds since the epoch
         */
        time_t timestamp() const;

        /**
         * \brief Queries the UTC flag
         *
         * See setUseUtc().
         *
         * \return \c true if UTC is used, \c false if localtime is used.
         */
        bool useUtc() const;

        /**
         * \brief Sets the utc flag
         *
         * This flag affects the day(), month(), year(), minute(), hour() and second() functions as
         * well as str().
         *
         * \param[in] use_utc \c true if UTC should be used, \c false if localtime should be used.
         */
        void setUseUtc(bool use_utc);

        /**
         * \brief Returns the day in the month
         *
         * \return the day, from 1 to 31.
         */
        int day() const;

        /**
         * \brief Returns the month
         *
         * \return the month, from 1 to 12.
         */
        int month() const;

        /**
         * \brief Returns the year
         *
         * \return the 4-digits year, e.g. 2010.
         */
        int year() const;

        /**
         * \brief Returns the hour
         *
         * \return the hour, from 0 to 23, e.g. 18
         */
        int hour() const;

        /**
         * \brief Returns the minute
         *
         * \return the minute, from 0 to 59, e.g. 58.
         */
        int minute() const;

        /**
         * \brief Returns the second
         *
         * \return the second, from 0 to 59, e.g. 50.
         */
        int second() const;

        /**
         * \brief Returns the weekday
         *
         * \return the weekday
         */
        Weekday weekday() const;

        /**
         * \brief Adds the given amount of days to the time value
         *
         * \param[in] days the number of days which may be negative
         * \return a self reference
         */
        Datetime &addDays(int days);

        /**
         * \brief Adds the given amount of hours to the time value
         *
         * \param[in] hours the number of hours which may be negative
         * \return a self reference
         */
        Datetime &addHours(int hours);

        /**
         * \brief Adds the given amount of minutes to the time value
         *
         * \param[in] minutes the number of minutes which may be negative
         * \return a self reference
         */
        Datetime &addMinutes(int minutes);

        /**
         * \brief Adds the given amount of seconds to the time value
         *
         * \param[in] secs the number of seconds which may be negative
         * \return a self reference
         */
        Datetime &addSeconds(int secs);

        /**
         * \brief Formats the time according to \p format
         *
         * See the documentation of your system's strftime() implementation for details.
         * If the locale of the program has been set, the output is locale dependent.
         *
         * \note The function is only available if the system has a strftime() implementation.
         *       Otherwise, the emtpy string is returned. The functions str() and dateStr()
         *       are always availabe.
         *
         * \param[in] format the format string, e.g. <tt>"%Y-%m-%d"</tt> for the
         *            ISO display.
         * \return the formated string.
         */
        std::string strftime(const char *format) const
        BW_COMPILER_STRFTIME_FORMAT(2, 0);

        /**
         * \brief Parses the time string \p time according to \p format and returns a Datetime object
         *
         * See the documentation of your system's strptime() implementation for details.
         * If the locale of the program has been set, the input is parsed according to that
         * locale settings.
         *
         * \note The function is only available if the system has a strptime() implementation.
         *       Otherwise, an invalid Datetime object is returned.
         *
         * \param[in] time the time that should be parsed, e.g. <tt>"2012-01-01"</tt> for
         *            January 1st in the ISO format.
         * \param[in] format a format string, e.g. <tt>"%Y-%m-%d"</tt> for an ISO date.
         * \param[in] isUtc \c true if the broken-up time is UTC, \c false if it's localtime.
         * \return a valid Datetime object on success, an invalid Datetime on error.
         */
        static Datetime strptime(const std::string &time, const char *format, bool isUtc=false)
        BW_COMPILER_STRFTIME_FORMAT(2, 0);

        /**
         * \brief Converts the datetime object to a human readable string
         *
         * \return the string in the ISO format, i.e. <tt>"%Y-%m-%d %H:%M:%s"</tt>.
         */
        std::string str() const;

        /**
         * \brief Converts the datetime object to a human readable date string
         *
         * \return the date string in the ISO format, i.e. <tt>"%Y-%m-%d"</tt>.
         */
        std::string dateStr() const;

        /**
         * \brief Calculates the seconds from \c this to \c time.
         *
         * To get a positive number, \p time must be behind \c this.
         *
         * \param[in] time the time object which is used for the calculation
         * \return the positive number of seconds that must be added to \c this to get \p time
         */
        long long secsTo(const Datetime &time) const;

        /**
         * \brief Compares two Datetime objects for equality
         *
         * Checks if two Datetime objects are equal. They are equal if timestamp() returns the same
         * number.
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if the two Datetime objects are equal, \c false otherwise.
         */
        bool operator==(const Datetime &other);

        /**
         * \brief Compares two Datetime for unequality
         *
         * Checks if two Datetime objects are not equal. They are not equal if timestamp() returns
         * different numbers.
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if the two Datetime objects are not equal, \c false otherwise.
         */
        bool operator!=(const Datetime &other);

        /**
         * \brief Compares two Datetime for "less than"
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if this is less than \p other, \c false otherwise.
         */
        bool operator<(const Datetime &other);

        /**
         * \brief Compares two Datetime for "less equal"
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if this is less than \p other, \c false otherwise.
         */
        bool operator<=(const Datetime &other);

        /**
         * \brief Compares two Datetime for "greater than"
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if this is greater than \p other, \c false otherwise.
         */
        bool operator>(const Datetime &other);

        /**
         * \brief Compares two Datetime for "greater equal"
         *
         * \param[in] other the Datetime object to compare with
         * \return \c true if this is greater than \p other, \c false otherwise.
         */
        bool operator>=(const Datetime &other);

    protected:
        /**
         * \brief Fills m_tm.
         *
         * This function needs to be called whenever the timestamp is changed or the UTC flag is
         * changed. Then all members in m_tm are re-calculated.
         *
         * \return a self reference
         */
        Datetime &fillTime();

    private:
        time_t      m_time;
        struct tm   m_tm;
        bool        m_useUtc;
};

/**
 * \brief Prints a Datetime object
 *
 * Uses Datetime::str().
 *
 * \param[in] os the output stream
 * \param[in] datetime the date/time object
 * \return the output stream
 */
std::ostream &operator<<(std::ostream &os, const Datetime &datetime);

/* }}} */

} // end namespace bw

#endif /* DATETIME_H */

// vim: set sw=4 ts=4 et fdm=marker:
