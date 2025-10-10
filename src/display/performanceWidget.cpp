#include "display/performanceWidget.h"
#include "display/text.h"
#include "display/alignment.h"
#include "hardware/display.h"
#include <vector>
#include <Arduino.h>

struct PerformanceMeasurement
{
    const char* name;
    uint32_t duration;
    const uint8_t *bitmap;
};

static std::vector<PerformanceMeasurement> measurements={};

void addPerformanceMeasurement(const char* name, uint32_t duration, const uint8_t *bitmap)
{
    for (PerformanceMeasurement &m : measurements)
    {
        if (strcmp(m.name, name) == 0)
        {
            m.duration += duration;
            return;
        }
    }
    measurements.push_back({name, duration, bitmap});
}
void drawPerformanceWidget(int16_t x, int16_t y)
{
    std::sort(measurements.begin(), measurements.end(), [](const PerformanceMeasurement &a, const PerformanceMeasurement &b) {
        return a.duration > b.duration;
    });
    for(int i=0; i<measurements.size(); i++)
    {
        PerformanceMeasurement &m = measurements[i];
        dp.drawBitmap(x, y + i * 8, m.bitmap, 7, 7, fg_color);
        textBox(String(m.duration)+"ms", x + 8, y + i * 8, 6*5, 8, TD_TOP_LEFT);
    }
    measurements.clear();
}