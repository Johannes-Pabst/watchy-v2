#pragma once

#include <vector>

#define LT_SHORTBREAK 0
#define LT_LONGBREAK 1
#define LT_LESSON 2
#define LT_END 2

struct LessonInfo{
    int endMin;
    int type;
};
struct TimeGridInfo{
    std::vector<LessonInfo> lessons;
};

struct TimeTableInfo{
    int firstLessonId;
    int lastLessonId;
};

extern TimeGridInfo timegridinfo;
extern TimeTableInfo timetableinfo;