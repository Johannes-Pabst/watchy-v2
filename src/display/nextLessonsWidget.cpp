#include "display/nextLessonsWidget.h"
#include "math/timeTable.h"
#include "hardware/display.h"
#include "display/text.h"
#include "display/alignment.h"

void nextLessonsWidget(int x, int y, int number){
    int curDayId=-1;
    for(int i=0; i<timetableinfo.days.size(); i++){
        if(timetableinfo.days[i].date == timetableinfo.date){
            curDayId = i;
            break;
        }
    }
    if(curDayId == -1){
        textBox("No lessons", x, y, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
        return;
    }
    DayInfo& curDay = timetableinfo.days[curDayId];
    for(int i=0; i<number; i++){
        int lessonIndex = curDayId + i;
        if(curDay.lessonTimeSlots.size()>lessonIndex){
            std::vector<LessonInfo>& lessons = curDay.lessonTimeSlots[lessonIndex];
            String lessonTimeStr = "";
            for(int j=0; j<lessons.size(); j++){
                LessonInfo& lesson = lessons[j];
                lessonTimeStr += lesson.subject + "@" + lesson.room+" ";
            }
            textBox(lessonTimeStr, x, y + i * 8, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
        }
    }
}