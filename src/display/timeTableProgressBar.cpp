#include "display/timeTableProgressBar.h"
#include "display/text.h"
#include "display/alignment.h"
#include "hardware/display.h"
#include "math/timeTable.h"
#include "math/time.h"

String formatSecDur(int secs)
{
    if (secs < 60)
    {
        return String(secs);
    }
    else if (secs % 60 == 0)
    {
        return String(secs / 60);
    }
    else
    {
        return String(secs / 60) + ":" + String(secs % 60);
    }
}
void timeTableProgressBar(int16_t x, int16_t y, int16_t w, int16_t h)
{
    if (shouldDisplaySchoolTime())
    {
        int startTime = timegridinfo.lessons[timetableinfo.timeGridId].startMin * 60;
        int endTime = timegridinfo.lessons[timetableinfo.timeGridId + 1].startMin * 60;
        int nowMinutes = getSecond();
        float progress = float(nowMinutes - startTime) / float(endTime - startTime);
        float nextProgress = float((nowMinutes - startTime + 60)/60*60) / float(endTime - startTime);
        int w1;
        textBox(String(startTime / 60 / 60) + ":" + String(startTime / 60 % 60) + (startTime % 60 == 0 ? "" : (":" + String(startTime % 60))), x, y, w, h, TD_TOP_LEFT, nullptr, 1, fg_color,&w1);
        int w2;
        textBox(String(endTime / 60 / 60) + ":" + String(endTime / 60 % 60) + (endTime % 60 == 0 ? "" : (":" + String(endTime % 60))), x, y, w, h, TD_TOP_RIGHT, nullptr, 1, fg_color,&w2);
        int w3;
        textBox(String(int(progress * 100)) + "%", x, y, w, h, TD_TOP_CENTER, nullptr, 1, fg_color,&w3);
        textBox(formatSecDur(nowMinutes - startTime), x+w1, y, w / 2-w3/2-w1, h, TD_TOP_CENTER, nullptr, 1, fg_color);
        textBox(formatSecDur(endTime - nowMinutes), x + w / 2+w3/2, y, w / 2-w3/2-w2, h, TD_TOP_CENTER, nullptr, 1, fg_color);
        dp.fillRect(x, y + 7, int(float(w) * progress), h - 7, fg_color);
        for (int ix = int(float(w) * progress); ix < w; ix++)
        {
            for (int iy = 0; iy < h - 7; iy++)
            {
                int m3 = (ix + iy) % 3;
                if (m3 == 0 || (m3 == 1 && ix < int(float(w) * nextProgress)))
                {
                    dp.drawPixel(x + ix, y + iy + 7, fg_color);
                }
            }
        }
    }
}