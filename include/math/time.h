#pragma once

#include <WatchyRTC.h>

struct SimpleTime { 
  int second; 
  int minute; 
  int hour; 
  int wday;   // day of week, sunday is day 1
  int day;
  int month; 
  int year;
};

void utcTo(SimpleTime &t);
void utcTo(SimpleTime &t, int offset, bool daylightSavingsTime);
/*ONLY WORKS FOR EU! stupid daylight savings time...*/
bool isDaylightSavingsTime(SimpleTime &t);
bool afterDSTswitch(int length, SimpleTime &t);
int getMonthLength(int month, int year);
bool isLeapYear(int year);
SimpleTime fromRtc(tmElements_t &t);
SimpleTime simpleNow();
SimpleTime simpleNowUTC();
void fixTime(SimpleTime &t);