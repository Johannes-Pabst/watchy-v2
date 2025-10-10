#include "display/SSOgraph.h"
#include "display/moonRender.h"
#include "display/text.h"
#include "display/alignment.h"
#include "math/sky.h"
#include "math/time.h"
#include "hardware/display.h"
#include "hardware/fs.h"
#include "images/SSOgraph.h"
#include "images/tinyFont.h"
#include <Arduino.h>
#include <LittleFS.h>

#define LIMIT_HOURS_TO_24(value) ((value) - floor((value) * (1.0 / 24.0)) * 24)

uint32_t time_test_save = 0;
#define TIME                                                                        \
    {                                                                               \
        Serial.println(String(millis() - time_test_save) + " at line " + __LINE__); \
        time_test_save = millis();                                                  \
    }

void clearSSOGraphCache()
{
    if (LittleFS.exists("/SSOgraph"))
    {
        removeDirRecursive("/SSOgraph");
    }
}
void SSOGraphWidget(int16_t x, int16_t y, int16_t width, int16_t height, SolarSystemObjectIndex obj, uint8_t text_pos)
{
    int16_t raw_x = x;
    int16_t raw_y = y;
    int16_t raw_width = width;
    int16_t raw_height = height;
    width -= 6; // extra space for sun/moon icon
    x += 3;
    height -= 6;
    y += 3;
    width -= 14 * 2; // extra space for legend
    x += 14;
    if (text_pos == TD1_START)
    {
        y += 8;
    }
    if (text_pos != TD1_CENTER)
    {
        height -= 8;
    }
    float deg[width] = {};
    SimpleTime t = simpleNow();
    String rootFolder = "/SSOgraph";
    String folderName = String(t.day) + "." + String(t.month) + "." + String(t.year);
    String path = rootFolder + "/" + folderName;
    String fileName = path + "/" + String(obj) + "-" + String(width);
    File exists=LittleFS.open(fileName);
    if (t.minute == 0 || !exists)
    {
        if (LittleFS.exists(rootFolder))
        {
            File root = LittleFS.open(rootFolder);
            File file = root.openNextFile();
            while (file)
            {
                if (String(file.name()) != folderName)
                {
                    removeDirRecursive(file.path());
                }
                file.close();
                file = root.openNextFile();
            }
            file.close();
            root.close();
        }
        else
        {
            LittleFS.mkdir(rootFolder);
        }
        if (!LittleFS.exists(path))
        {
            LittleFS.mkdir(path);
        }
    }
    if (exists)
    {
        exists.readBytes((char *)deg, sizeof(float) * width);
    }
    else
    {
        Serial.println("Calculating SSO graph for " + fileName);
        for (int i = 0; i < width; i++)
        {
            float frac = i / float(width - 1);
            int seconds = frac * 24 * 60 * 60;
            SimpleTime tu = simpleNowUTC();
            tu.hour -= t.hour;
            tu.minute = 0;
            tu.second = seconds;
            fixTime(tu);
            SolarSystemObject s = calculatePos(obj, &tu);
            deg[i] = s.horiCoordinates.alt;
        }
        File file = LittleFS.open(fileName, "w");
        file.write((uint8_t *)deg, sizeof(float) * width);
        file.close();
    }
    float maxDeg = -std::numeric_limits<float>::infinity();
    float minDeg = std::numeric_limits<float>::infinity();
    for (int i = 0; i < width; i++)
    {
        maxDeg = max(maxDeg, deg[i]);
        minDeg = min(minDeg, deg[i]);
    }
    float zero = (height + minDeg / (maxDeg - minDeg) * height);
    dp.drawLine(x, y + zero, x + width, y + zero, fg_color);
    for (int i = 0; i < width - 1; i++)
    {
        dp.drawLine(x + i, y + (height - (deg[i] - minDeg) / (maxDeg - minDeg) * height), x + i + 1, y + (height - (deg[i + 1] - minDeg) / (maxDeg - minDeg) * height), fg_color);
    }
    float currentFrac = float(((t.hour) * 60 + t.minute) * 60 + t.second) / 60.0 / 60.0 / 24.0;
    int currentX = currentFrac * float(width);
    int currentY = (height - (deg[currentX] - minDeg) / (maxDeg - minDeg) * height);
    if (obj == EarthsMoon)
    {
        dp.fillCircle(currentX + x, currentY + y, 3, bg_color);
        moonRenderWidget(currentX + x, currentY + y, 3);
    }
    else if (obj == Sun)
    {
        dp.drawBitmap(currentX + x - 3, currentY + y - 3, epd_bitmap_sun, 7, 7, fg_color);
    }
    else
    {
        dp.fillCircle(currentX + x, currentY + y, 3, fg_color);
    }
    SolarSystemObject *s = getCurrent(obj);
    dp.drawBitmap(x - 2 - 3, currentY + y - 1, epd_bitmap_arr_right, 2, 3, fg_color);
    textBox(String((int)round(abs(s->horiCoordinates.alt))) + "@", x - 14 - 3, currentY + y - 2, 12, 6, TD_TOP_RIGHT, &tinyFont);
    dp.drawBitmap(x + width + 3 + 1, y - 1, epd_bitmap_arr_left, 2, 3, fg_color);
    textBox(String((int)round(abs(maxDeg))) + "@", x + width + 3 + 2 + 2, y - 2, 12, 6, TD_TOP_LEFT, &tinyFont);
    dp.drawBitmap(x + width + 3 + 1, y + height - 1, epd_bitmap_arr_left, 2, 3, fg_color);
    textBox(String((int)round(abs(minDeg))) + "@", x + width + 3 + 2 + 2, y + height - 1 - 1, 12, 6, TD_TOP_LEFT, &tinyFont);
    dp.drawBitmap(x + width + 3 + 1, y + zero - 1, epd_bitmap_arr_left, 2, 3, fg_color);
    textBox("0@", x + width + 3 + 2 + 2, y + zero - 2, 12, 6, TD_TOP_LEFT, &tinyFont);
    if (text_pos == TD1_START)
    {
        if (LIMIT_HOURS_TO_24(s->rise) < LIMIT_HOURS_TO_24(s->set))
        {
            textBox(doubleToTimeString(s->rise), raw_x, raw_y, raw_width + 1, 8, TD_TOP_LEFT);
            textBox(doubleToTimeString(s->set), raw_x, raw_y, raw_width + 1, 8, TD_TOP_RIGHT);
            dp.drawBitmap(((raw_x + 6 * 5) + (raw_x + raw_width - 6 * 5 - 4)) / 2, raw_y, epd_bitmap_arr_ud, 5, 7, fg_color);
        }
        else
        {
            textBox(doubleToTimeString(s->set), raw_x, raw_y, raw_width + 1, 8, TD_TOP_LEFT);
            textBox(doubleToTimeString(s->rise), raw_x, raw_y, raw_width + 1, 8, TD_TOP_RIGHT);
            dp.drawBitmap(((raw_x + 6 * 5) + (raw_x + raw_width - 6 * 5 - 4)) / 2, raw_y, epd_bitmap_arr_du, 5, 7, fg_color);
        }
    }
    else if (text_pos == TD1_END)
    {
        if (LIMIT_HOURS_TO_24(s->rise) < LIMIT_HOURS_TO_24(s->set))
        {
            textBox(doubleToTimeString(s->rise), raw_x, raw_y + raw_height - 7, raw_width + 1, 8, TD_TOP_LEFT);
            textBox(doubleToTimeString(s->set), raw_x, raw_y + raw_height - 7, raw_width + 1, 8, TD_TOP_RIGHT);
            dp.drawBitmap(((raw_x + 6 * 5) + (raw_x + raw_width - 6 * 5 - 4)) / 2, raw_y + raw_height - 7, epd_bitmap_arr_ud, 5, 7, fg_color);
        }
        else
        {
            textBox(doubleToTimeString(s->set), raw_x, raw_y + raw_height - 7, raw_width + 1, 8, TD_TOP_LEFT);
            textBox(doubleToTimeString(s->rise), raw_x, raw_y + raw_height - 7, raw_width + 1, 8, TD_TOP_RIGHT);
            dp.drawBitmap(((raw_x + 6 * 5) + (raw_x + raw_width - 6 * 5 - 4)) / 2, raw_y + raw_height - 7, epd_bitmap_arr_du, 5, 7, fg_color);
        }
    }
}
String doubleToTimeString(double time)
{
    int hours, minutes;
    FLOAT seconds;
    Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(time, 0), &hours, &minutes, &seconds);
    SimpleTime t = simpleNowUTC();
    t.hour = hours;
    t.minute = minutes;
    t.second = seconds;
    utcTo(t);
    return String(t.hour < 10 ? "0" : "") + String(t.hour) + ":" + String(t.minute < 10 ? "0" : "") + String(t.minute);
}