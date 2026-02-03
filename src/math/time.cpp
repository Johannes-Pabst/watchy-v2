#include "math/time.h"
#include "math/config.h"
#include "hardware/rtc.h"
#include <WatchyRTC.h>
#include "time.h"

void utcTo(SimpleTime &t, int offset, bool daylightSavingsTime)
{
    t.hour += offset;
    fixTime(t);
    if (daylightSavingsTime && isDaylightSavingsTime(t))
    {
        t.hour += 1;
        fixTime(t);
    }
}
int utcOffset;
bool eu_dst;
int time_config_initialized=false;
int getUtcOffset()
{
    if (!time_config_initialized)
    {
        JSONVar cfg = parseCategory("Location", config);
        utcOffset = parseInt("UTC Offset", cfg);
        eu_dst = parseCheckbox("EU daylight savings time", cfg);
        time_config_initialized = true;
    }
    return utcOffset;
}
void utcTo(SimpleTime &t)
{
    utcTo(t, getUtcOffset(), eu_dst);
}
bool isDaylightSavingsTime(SimpleTime &t)
{
    if (t.month > 3 && t.month < 10)
    {
        return true;
    }
    if (t.month < 3 || t.month > 10)
    {
        return false;
    }
    if (t.month == 3)
    {
        return afterDSTswitch(31, t);
    }
    if (t.month == 10)
    {
        return !afterDSTswitch(31, t);
    }
    return false; // how did we get here?
}
bool afterDSTswitch(int length, SimpleTime &t)
{
    int daysTillLast = length - t.day;
    int daysTillSunday = 6 - t.wday;
    if (daysTillSunday == 0 && daysTillLast < 7)
    {
        if (t.hour > 2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    if (daysTillLast >= daysTillSunday)
    {
        return false;
    }
    else
    {
        return true;
    }
}
void fixTime(SimpleTime &t)
{
    if (t.second >= 60)
    {
        t.minute += t.second / 60;
        t.second %= 60;
    }
    if (t.second < 0)
    {
        int temp = (-t.second) / 60 + 1;
        t.minute -= temp;
        t.second += 60 * temp;
    }
    if (t.minute >= 60)
    {
        t.hour += t.minute / 60;
        t.minute %= 60;
    }
    if (t.minute < 0)
    {
        int temp = (-t.minute) / 60 + 1;
        t.hour -= temp;
        t.minute += 60 * temp;
    }
    if (t.hour >= 24)
    {
        t.day += t.hour / 24;
        t.hour %= 24;
    }
    if (t.hour < 0)
    {
        int temp = (-t.hour) / 24 + 1;
        t.day -= temp;
        t.wday -= temp;
        t.hour += 24 * temp;
    }
    if (t.wday < 0)
    {
        t.wday += ((-t.wday) / 7 + 1) * 7;
    }
    t.wday %= 7;
    while (t.day > getMonthLength(t.month, t.year))
    {
        t.day -= getMonthLength(t.month, t.year);
        t.month += 1;
    }
    while (t.day < 1)
    {
        t.day += getMonthLength(t.month - 1, t.year);
        t.month -= 1;
    }
    if (t.month < 0)
    {
        t.month += ((-t.month) / 12 + 1) * 12;
    }
    t.month %= 12;
    if (t.month == 0)
    {
        t.month = 12;
    }
}
int getMonthLength(int month, int year)
{
    if (month < 0)
    {
        month += ((-month) / 12 + 1) * 12;
    }
    month %= 12;
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 0)
    {
        return 31;
    }
    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    if (month == 2)
    {
        if (isLeapYear(year))
        {
            return 29;
        }
        else
        {
            return 28;
        }
    }
    return 31; // how did we get here?
}
bool isLeapYear(int year)
{
    return (((year % 4) == 0) && ((year % 100) != 0)) || ((year % 400) == 0);
}
SimpleTime fromRtc(tmElements_t &t)
{
    return {t.Second, t.Minute, t.Hour, (t.Wday + 5) % 7, t.Day, t.Month, t.Year + 1970};
}
SimpleTime simpleNow()
{
    SimpleTime utc = simpleNowUTC();
    utcTo(utc);
    return utc;
}
SimpleTime simpleNowUTC()
{
    tmElements_t time;
    RTC.read(time);
    return fromRtc(time);
}
long getUnixTimestamp(SimpleTime &t){
    long timestamp = 0;
    timestamp+= t.second;
    timestamp+= t.minute * 60;
    timestamp+= t.hour * 3600;
    timestamp+= (t.day -1) * 86400;
    for(int m=1; m<t.month; m++){
        timestamp+= getMonthLength(m, t.year) * 86400;
    }
    for(int y=1970; y<t.year; y++){
        if(isLeapYear(y)){
            timestamp+= 366 * 86400;
        } else {
            timestamp+= 365 * 86400;
        }
    }
    return timestamp;
}