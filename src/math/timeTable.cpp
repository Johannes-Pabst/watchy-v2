#include "math/timeTable.h"
#include "math/time.h"
#include "LittleFS.h"
#include "math/padUtils.h"
#include <Arduino.h>
#include <vector>

TimeGridInfo timegridinfo = TimeGridInfo{
    std::vector<TimeslotInfo>{
        TimeslotInfo{60 * 7 + 45, LT_SHORTBREAK},
        TimeslotInfo{60 * 7 + 55, LT_LESSON},
        TimeslotInfo{60 * 8 + 38, LT_SHORTBREAK},
        TimeslotInfo{60 * 8 + 42, LT_LESSON},
        TimeslotInfo{60 * 9 + 25, LT_LONGBREAK},
        TimeslotInfo{60 * 9 + 35, LT_LESSON},
        TimeslotInfo{60 * 10 + 23, LT_SHORTBREAK},
        TimeslotInfo{60 * 10 + 27, LT_LESSON},
        TimeslotInfo{60 * 11 + 10, LT_LONGBREAK},
        TimeslotInfo{60 * 11 + 25, LT_LESSON},
        TimeslotInfo{60 * 12 + 13, LT_SHORTBREAK},
        TimeslotInfo{60 * 12 + 17, LT_LESSON},
        TimeslotInfo{60 * 13 + 0, LT_LONGBREAK},
        TimeslotInfo{60 * 13 + 15, LT_LESSON},
        TimeslotInfo{60 * 13 + 55, LT_LESSON},
        TimeslotInfo{60 * 14 + 40, LT_LESSON},
        TimeslotInfo{60 * 15 + 30, LT_END},
    }};

TimeTableInfo timetableinfo;
bool timetinfinstantiated = false;

void parseLesson(String lstart, String lend, String ldate, String lsubject, String lroom, String lteacher, String lcode)
{
    int lessonId = lstart.toInt();
    {
        int end = lend.toInt();
        if (lessonId < end)
        {
            for (int i = lessonId; i <= end; i++)
            {
                parseLesson(String(i), String(i), ldate, lsubject, lroom, lteacher, lcode);
            }
            return;
        }
    }
    int dayId = timetableinfo.days.size();
    for (int i = 0; i < timetableinfo.days.size(); i++)
    {
        if (timetableinfo.days[i].date == ldate)
        {
            dayId = i;
        }
    }
    if (dayId == timetableinfo.days.size())
    {
        timetableinfo.days.push_back(DayInfo{
            INT_MAX,
            -1,
            ldate,
            std::vector<std::vector<LessonInfo>>{},
        });
    }
    bool mandatory = !lcode.equals("canc.") && !lcode.equals("EVA");
    if (mandatory)
    {
        timetableinfo.days[dayId].firstMandatoryLessonId = min(timetableinfo.days[dayId].firstMandatoryLessonId, lessonId);
        timetableinfo.days[dayId].lastMandatoryLessonId = max(timetableinfo.days[dayId].lastMandatoryLessonId, lessonId);
    }
    while (timetableinfo.days[dayId].lessonTimeSlots.size() <= lessonId)
    {
        timetableinfo.days[dayId].lessonTimeSlots.push_back(std::vector<LessonInfo>{});
    }
    timetableinfo.days[dayId].lessonTimeSlots[lessonId].push_back(LessonInfo{
        lsubject,
        lroom,
        lteacher,
        lcode,
    });
}

void parseTimeT()
{
    timetableinfo = TimeTableInfo{
        -1,
        0,
        "",
        std::vector<DayInfo>{},
    };
    SimpleTime t = simpleNow();
    int minute = t.hour * 60 + t.minute;
    // int minute=8*60+30; // for testing
    timetableinfo.date = String(t.year) + padLeft(String(t.month), 2) + padLeft(String(t.day), 2);
    for (int i = 0; i < timegridinfo.lessons.size(); i++)
    {
        if (minute > timegridinfo.lessons[i].startMin)
        {
            timetableinfo.timeGridId++;
            ;
            if (timegridinfo.lessons[i].type == LT_LESSON)
            {
                timetableinfo.lessonId++;
            }
        }
        else
        {
            break;
        }
    }
    File file = LittleFS.open("/timet");
    if (!file)
        return;
    char buffer[20];
    int bufCurLen = 0;
    int bufId = 0;
    int sstate = 0;
    String lstart = "";
    String lend = "";
    String ldate = "";
    String lsubject = "";
    String lroom = "";
    String lteacher = "";
    String lcode = "";
    while (true)
    {
        if (bufId >= bufCurLen)
        {
            bufCurLen = file.readBytes(buffer, 20);
            bufId = 0;
            if (bufCurLen == 0)
                break;
        }
        char c = buffer[bufId];

        if (c == '\\')
        {
            sstate = (sstate + 1) % 7;
            if (sstate == 0)
            {
                parseLesson(lstart, lend, ldate, lsubject, lroom, lteacher, lcode);
                lstart = "";
                lend = "";
                ldate = "";
                lsubject = "";
                lroom = "";
                lteacher = "";
                lcode = "";
            }
        }
        else
        {
            switch (sstate)
            {
            case 0:
                lstart += c;
                break;
            case 1:
                lend += c;
                break;
            case 2:
                ldate += c;
                break;
            case 3:
                lsubject += c;
                break;
            case 4:
                lroom += c;
                break;
            case 5:
                lteacher += c;
                break;
            case 6:
                lcode += c;
                break;
            }
        }

        bufId++;
    }
    file.close();
    timetinfinstantiated = true;
}
void parseTimeTLazy(){
    if(!timetinfinstantiated){
        parseTimeT();
    }
}
bool isSchool(){
    parseTimeTLazy();
    return timetableinfo.timeGridId>=0&&timetableinfo.timeGridId<timegridinfo.lessons.size()-1;
}
/* 
better protocoll:

u16 lesson#
u32 days since 1970
u16 subjId
u16 roomId
u16 teacherId
u16 flags:
u1  cancelled
u1  hasMessage
u1  hasHomework
...
u16 flagStrStartId
*/