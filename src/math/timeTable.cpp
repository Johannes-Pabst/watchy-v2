#include "math/timeTable.h"
#include "LittleFS.h"
#include <Arduino.h>
#include <vector>

TimeGridInfo timegridinfo = TimeGridInfo{
    std::vector<LessonInfo>{
        LessonInfo{60 * 7 + 45, LT_SHORTBREAK},
        LessonInfo{60 * 7 + 55, LT_LESSON},
        LessonInfo{60 * 8 + 38, LT_SHORTBREAK},
        LessonInfo{60 * 8 + 42, LT_LESSON},
        LessonInfo{60 * 9 + 25, LT_LONGBREAK},
        LessonInfo{60 * 9 + 35, LT_LESSON},
        LessonInfo{60 * 10 + 23, LT_SHORTBREAK},
        LessonInfo{60 * 10 + 27, LT_LESSON},
        LessonInfo{60 * 11 + 10, LT_LONGBREAK},
        LessonInfo{60 * 11 + 25, LT_LESSON},
        LessonInfo{60 * 12 + 13, LT_SHORTBREAK},
        LessonInfo{60 * 12 + 17, LT_LESSON},
        LessonInfo{60 * 13 + 0, LT_LONGBREAK},
        LessonInfo{60 * 13 + 15, LT_LESSON},
        LessonInfo{60 * 13 + 55, LT_LESSON},
        LessonInfo{60 * 14 + 40, LT_LESSON},
        LessonInfo{60 * 15 + 30, LT_END},
    }};

void parseLesson(String lstart, String lend, String ldate, String lsubject, String lroom, String lteacher, String lcode)
{
}

void parseTimeT()
{
    File file = LittleFS.open("timet");
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
}