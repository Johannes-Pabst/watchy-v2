#include "display/timeTableProgressBar.h"
#include "display/text.h"
#include "display/alignment.h"
#include "hardware/display.h"
#include "math/timeTable.h"
#include "math/time.h"

void timeTableProgressBar(int16_t x, int16_t y, int16_t w, int16_t h){
    if(isSchool()){
        int startTime=timegridinfo.lessons[timetableinfo.timeGridId].startMin;
        int endTime=timegridinfo.lessons[timetableinfo.timeGridId+1].startMin;
        SimpleTime now=simpleNow();
        int nowMinutes=now.hour*60+now.minute;
        float progress=float(nowMinutes-startTime)/float(endTime-startTime);
        float nextProgress=float(nowMinutes-startTime+1)/float(endTime-startTime);
        textBox(String(startTime/60)+":"+String(startTime%60), x, y, w, h, TD_TOP_LEFT, nullptr, 1, fg_color);
        textBox(String(endTime/60)+":"+String(endTime%60), x, y, w, h, TD_TOP_RIGHT, nullptr, 1, fg_color);
        textBox(String(int(progress*100))+"%", x, y, w, h, TD_TOP_CENTER, nullptr, 1, fg_color);
        textBox(String(nowMinutes-startTime), x, y, w/2, h, TD_TOP_CENTER, nullptr, 1, fg_color);
        textBox(String(endTime-nowMinutes), x+w/2, y, w/2, h, TD_TOP_CENTER, nullptr, 1, fg_color);
        dp.fillRect(x, y+7, int(round(float(w)*progress)), h-7, fg_color);
        for(int ix=0;ix<int(round(float(w)*progress));ix++){
            for(int iy=0;iy<h-7;iy++){
                int m3=(ix+iy)%3;
                if(m3==0||(m3==1&&))
                dp.drawPixel(x+ix, y+iy+7, bg_color);
            }
        }
    }
}