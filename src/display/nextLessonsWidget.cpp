#include "display/nextLessonsWidget.h"
#include "math/timeTable.h"
#include "hardware/display.h"
#include "display/text.h"
#include "display/alignment.h"

void nextLessonsWidget(int x, int y, int number)
{
    parseTimeTLazy();
    int curDayId = -1;
    for (int i = 0; i < timetableinfo.days.size(); i++)
    {
        if (timetableinfo.days[i].date == timetableinfo.date)
        {
            curDayId = i;
            break;
        }
    }
    if (curDayId == -1)
    {
        textBox("No lessons", x, y, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
        return;
    }
    DayInfo &curDay = timetableinfo.days[curDayId];
    int timeGridId = timetableinfo.timeGridId;
    for (int i = 0; i < number; i++)
    {
        if (timegridinfo.lessons.size() <= timeGridId)
        {
            break;
        }
        int type=timegridinfo.lessons[timeGridId].type;
        if (type == LT_LESSON){
            int lessonIndex = 0;
            for(int j=0;j<=timeGridId;j++){
                if(timegridinfo.lessons[j].type==LT_LESSON){
                    lessonIndex++;
                }
            }
            std::vector<LessonInfo> &lessons = curDay.lessonTimeSlots[lessonIndex];
            String lessonTimeStr = "";
            for (int j = 0; j < lessons.size(); j++)
            {
                LessonInfo &lesson = lessons[j];
                lessonTimeStr += lesson.subject + "@" + lesson.room + " ";
            }
            textBox("l"+String(lessonIndex)+" "+lessonTimeStr, x, y + i * 8, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
        }else if(type==LT_LONGBREAK){
            int breakIndex = 0;
            for(int j=0;j<=timeGridId;j++){
                if(timegridinfo.lessons[j].type==LT_LONGBREAK){
                    breakIndex++;
                }
            }
            textBox("b"+String(breakIndex), x, y + i * 8, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
        }
        timeGridId++;
        while (timegridinfo.lessons.size() > timeGridId && (timegridinfo.lessons[timeGridId].type == LT_SHORTBREAK||timegridinfo.lessons[timeGridId].type == LT_END))
        {
            timeGridId++;
        }
    }
}