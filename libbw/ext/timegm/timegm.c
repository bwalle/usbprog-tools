
#include "timegm.h"

static int is_leap(unsigned y)
{
	y += 1900;
	return (y % 4) == 0 && ((y % 100) != 0 || (y % 400) == 0);
}

time_t timegm(struct tm *tm)
{
	static const unsigned ndays[2][12] ={
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
	time_t res = 0;
	unsigned i;

	if (tm->tm_mon > 12 ||
	    tm->tm_mon < 0 ||
	    tm->tm_mday > 31 ||
	    tm->tm_min > 60 ||
	    tm->tm_sec > 60 ||
	    tm->tm_hour > 24) {
		/* invalid tm structure */
		return 0;
	}
	
	for (i = 70; i < tm->tm_year; ++i)
		res += is_leap(i) ? 366 : 365;
	
	for (i = 0; i < tm->tm_mon; ++i)
		res += ndays[is_leap(tm->tm_year)][i];
	res += tm->tm_mday - 1;
	res *= 24;
	res += tm->tm_hour;
	res *= 60;
	res += tm->tm_min;
	res *= 60;
	res += tm->tm_sec;
	return res;
}
