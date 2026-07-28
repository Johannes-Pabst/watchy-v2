#include "display/stepsWidget.h"
#include "hardware/bma.h"
#include "hardware/display.h"
#include "hardware/fs.h"
#include "display/text.h"
#include "display/alignment.h"
#include "math/padUtils.h"
#include "math/time.h"
#include "images/performance.h"
#include <LittleFS.h>

void clearSteps()
{
    if (LittleFS.exists("/steps"))
    {
        removeDirRecursive("/steps");
    }
}
RTC_SLOW_ATTR int today; // days since 1970
RTC_SLOW_ATTR int totalSteps;
const int graphLen = 12;
RTC_SLOW_ATTR int graph[graphLen];
// 38 * 8
void stepsWidget(int16_t x, int16_t y)
{
    if (!LittleFS.exists("/steps/"))
    {
        LittleFS.mkdir("/steps/");
    }
    SimpleTime now = simpleNow();
    int days = getDays(now);
    Serial.println(days);
    int steps = getSteps();
    if (today != days)
    {
        for (int i = 0; i < 10; i++)
        {
            if (LittleFS.exists("/steps/" + String(days + i - graphLen + 1)))
            {
                File f = LittleFS.open("/steps/" + String(days + i - graphLen + 1));
                int temp;
                f.readBytes((char *)&temp, sizeof(int));
                f.close();
                graph[i] = temp;
                Serial.println("read: "+String(temp)+" at day "+String(days+i-graphLen+1));
            }
        }
        if (LittleFS.exists("/steps/total"))
        {
            File f = LittleFS.open("/steps/total");
            f.readBytes((char *)&totalSteps, sizeof(int));
            f.close();
        }
        today = days;
    }
    totalSteps += steps;
    graph[graphLen - 1] += steps;
    steps = graph[graphLen - 1];
    // int todayFile = 0;
    // if (LittleFS.exists("/steps/today"))
    // {
    //     File f = LittleFS.open("/steps/today");
    //     f.readBytes((char *)&todayFile, sizeof(int));
    //     f.close();
    // }
    // if (todayFile != days)
    // {
    //     LittleFS.remove("/steps/today");
    //     File f = LittleFS.open("/steps/today", "w", true);
    //     f.write((uint8_t *)&days, sizeof(int));
    //     f.close();
    // }
    {
        LittleFS.remove("/steps/" + String(days));
        File f = LittleFS.open("/steps/" + String(days), "w", true);
        f.write((uint8_t *)&graph[graphLen - 1], sizeof(int));
        f.close();
    }
    {
        LittleFS.remove("/steps/total");
        File f = LittleFS.open("/steps/total", "w", true);
        f.write((uint8_t *)&totalSteps, sizeof(int));
        f.close();
    }
    int graphHeight = 15;
    int maxSteps = 0;
    for (int i = 0; i < graphLen; i++)
    {
        if (graph[i] > maxSteps)
        {
            maxSteps = graph[i];
        }
    }
    resetSteps();
    if (maxSteps > 0)
    {
        for (int i = 0; i < graphLen; i++)
        {
            for (int j = 0; j < graph[i] * graphHeight / maxSteps; j++)
            {
                dp.drawPixel(x + i, y + graphHeight - j - 1, fg_color);
            }
        }
    }
    textBox(padLeft(String(maxSteps), 4), x + graphLen + 1, y, 4 * 6, 8, TD_TOP_LEFT);
    textBox(padLeft(String(steps), 4), x + graphLen + 1, y + 8, 4 * 6, 8, TD_TOP_LEFT);
    textBox(padLeft(String(totalSteps), 6), x, y + 16, 6 * 6, 8, TD_TOP_LEFT);
    dp.drawBitmap(x + 6 * 6 + 1, y, epd_bitmap_stepsWidget, 7, 7, fg_color);
}
// graph, streak, total, today