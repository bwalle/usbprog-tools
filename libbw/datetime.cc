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
#include <cstring>
#include <cassert>
#include <cstdio>

#include "datetime.h"
#include "bwconfig.h"

#ifndef HAVE_TIMEGM
#  include "timegm.h"
#endif

namespace bw {

/* gmtime_r() / localtime_r() {{{ */

#if !defined(HAVE_GMTIME_R) && !defined(gmtime_r)
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
    assert(timep != NULL);
    assert(result != NULL);

    *result = *(gmtime(timep));
    return result;
}
#endif // !defined(HAVE_GMTIME_R) && !defined(gmtime_r)

#if !defined(HAVE_LOCALTIME_R) && !defined(localtime_r)
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    assert(timep != NULL);
    assert(result != NULL);

    *result = *(localtime(timep));
    return result;
}
#endif // HAVE_LOCALTIME_R

/* }}} */
/* Datetime {{{ */

Datetime::Datetime()
    : m_time(0)
    , m_useUtc(false)
{}

Datetime::Datetime(const time_t &time)
    : m_time(time)
    , m_useUtc(false)
{
    localtime_r(&time, &m_tm);
}

Datetime::Datetime(int year, int month, int day, int hour, int minute, int second, bool utc)
    : m_useUtc(false)
{
    struct tm tm;

    memset(&tm, 0, sizeof(struct tm));
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = minute;
    tm.tm_sec = second;
    tm.tm_isdst = -1;

    if (utc)
        m_time = timegm(&tm);
    else
        m_time = mktime(&tm);

    localtime_r(&m_time, &m_tm);
}

Datetime Datetime::now()
{
    return Datetime( std::time(NULL) );
}

time_t Datetime::timestamp() const
{
    return m_time;
}

bool Datetime::useUtc() const
{
    return m_useUtc;
}

void Datetime::setUseUtc(bool use_utc)
{
    m_useUtc = use_utc;
    if (m_useUtc)
        gmtime_r(&m_time, &m_tm);
    else
        localtime_r(&m_time, &m_tm);
}

int Datetime::day() const
{
    return m_tm.tm_mday;
}

int Datetime::month() const
{
    return m_tm.tm_mon + 1;
}

int Datetime::year() const
{
    return m_tm.tm_year + 1900;
}

int Datetime::hour() const
{
    return m_tm.tm_hour;
}

int Datetime::minute() const
{
    return m_tm.tm_min;
}

int Datetime::second() const
{
    return m_tm.tm_sec;
}

Datetime::Weekday Datetime::weekday() const
{
    // we use 7 for Sunday, not 0
    return (m_tm.tm_wday == 0) ? Sunday : static_cast<Weekday>(m_tm.tm_wday);
}

Datetime &Datetime::addDays(int days)
{
    m_tm.tm_mday += days;
    return fillTime();
}

Datetime &Datetime::addHours(int hours)
{
    m_tm.tm_hour += hours;
    return fillTime();
}

Datetime &Datetime::addMinutes(int minutes)
{
    m_tm.tm_min += minutes;
    return fillTime();
}

Datetime &Datetime::addSeconds(int secs)
{
    m_tm.tm_sec += secs;
    return fillTime();
}

#ifdef HAVE_STRPTIME
Datetime Datetime::strptime(const std::string &time, const char *format, bool isUtc)
{
    struct tm timebuf;
    memset(&timebuf, 0, sizeof(struct tm));
    if (!::strptime(time.c_str(), format, &timebuf))
        return Datetime();

    return Datetime(timebuf.tm_year+1900, timebuf.tm_mon+1, timebuf.tm_mday,
                    timebuf.tm_mon, timebuf.tm_min, timebuf.tm_sec, isUtc);
}
#else
Datetime Datetime::strptime(const std::string &time, const char *format, bool isUtc)
{
    return Datetime();
}
#endif

#ifdef HAVE_STRFTIME
std::string Datetime::strftime(const char *format) const
{
    char buffer[BUFSIZ];
    ::strftime(buffer, BUFSIZ, format, &m_tm);
    return std::string(buffer);
}
#endif

std::string Datetime::str() const
{
    char buffer[80];
    std::sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
             year(), month(), day(), hour(), minute(), second());

    return std::string(buffer);
}

std::string Datetime::dateStr() const
{
    char buffer[11];
    std::sprintf(buffer, "%04d-%02d-%02d", year(), month(), day());
    return std::string(buffer);
}

long long Datetime::secsTo(const Datetime &time) const
{
    double diff = difftime(time.m_time, m_time);
    return static_cast<long long>(diff);
}

bool Datetime::operator==(const Datetime &other)
{
    return m_time == other.m_time;
}

bool Datetime::operator!=(const Datetime &other)
{
    return m_time != other.m_time;
}

bool Datetime::operator<(const Datetime &other)
{
    return m_time < other.m_time;
}

bool Datetime::operator<=(const Datetime &other)
{
    return m_time <= other.m_time;
}

bool Datetime::operator>(const Datetime &other)
{
    return m_time > other.m_time;
}

bool Datetime::operator>=(const Datetime &other)
{
    return m_time >= other.m_time;
}

Datetime &Datetime::fillTime()
{
    m_tm.tm_isdst = -1; // we need to recompute the DST flag
    if (m_useUtc)
        m_time = timegm(&m_tm);
    else
        m_time = mktime(&m_tm);

    return *this;
}


/* }}} */

} // end namespace bw

std::ostream &operator<<(std::ostream &os, const bw::Datetime &datetime)
{
    os << datetime.str();
    return os;
}

// vim: set sw=4 ts=4 et fdm=marker:
