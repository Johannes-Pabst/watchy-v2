#pragma once

#include <vector>
#include <Arduino.h>

#define LT_SHORTBREAK 0
#define LT_LONGBREAK 1
#define LT_LESSON 2
#define LT_END 3

struct TimeslotInfo
{
    int startMin;
    int type;
};

struct TimeGridInfo
{
    std::vector<TimeslotInfo> lessons;
};

struct LessonInfo
{
    String subject;
    String room;
    String teacher;
    String code;
};

struct DayInfo{
    int firstMandatoryLessonId; // 1-based
    int lastMandatoryLessonId;
    String date;
    std::vector<std::vector<LessonInfo>> lessonTimeSlots;
};

struct TimeTableInfo
{
    int timeGridId;
    int lessonId; // 1 means first lesson has started
    String date;
    std::vector<DayInfo> days;
};

extern TimeGridInfo timegridinfo;
extern TimeTableInfo timetableinfo;

void parseLesson(String lstart, String lend, String ldate, String lsubject, String lroom, String lteacher, String lcode);
void parseTimeT();
void parseTimeTLazy();
bool isSchool();
int getMinute();