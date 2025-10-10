#include "display/compass.h"
#include "math/sky.h"
#include "hardware/display.h"
#include "images/compass.h"
#include <Arduino.h>
#include <Ephemeris.hpp>

void betterFillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
    int16_t minx = min(x0, min(x1, x2));
    int16_t maxx = max(x0, max(x1, x2));
    int16_t miny = min(y0, min(y1, y2));
    int16_t maxy = max(y0, max(y1, y2));
    for (int16_t y = miny; y <= maxy; y++)
    {
        for (int16_t x = minx; x <= maxx; x++)
        {
            int64_t a = (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
            int64_t b = (x2 - x1) * (y - y1) - (y2 - y1) * (x - x1);
            int64_t c = (x0 - x2) * (y - y2) - (y0 - y2) * (x - x2);
            if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
            {
                dp.drawPixel(x, y, color);
            }
        }
    }
}
void compassWidget(int16_t x, int16_t y, int16_t radius, SolarSystemObjectIndex obj)
{
    SolarSystemObject *s = getCurrent(obj);
    double heading = s->horiCoordinates.azi * DEG_TO_RAD;
    // 0° => North on top
    // 90° => North to the right
    double middle_radius = radius - 8; // ~ sqrt(2)*5
    double inner_radius = radius / 4;
    double nx = sin(heading);
    double ny = cos(heading);
    double n45x = sin(heading + PI / 4);
    double n45y = cos(heading + PI / 4);
    for (int i = 0; i < 4; i++)
    {
        dp.drawLine(x, y, round(x + n45x * inner_radius), round(y + n45y * inner_radius), fg_color);
        dp.drawLine(round(x + n45x * inner_radius), round(y + n45y * inner_radius), round(x + nx * middle_radius), round(y + ny * middle_radius), fg_color);
        betterFillTriangle(round(x + nx * middle_radius), round(y + ny * middle_radius), round(x + n45x * inner_radius), round(y + n45y * inner_radius), x, y, fg_color);
        dp.drawLine(round(x + nx * middle_radius), round(y + ny * middle_radius), round(x - n45y * inner_radius), round(y + n45x * inner_radius), fg_color);
        double lx = nx * ((radius + middle_radius) / 2+1) + x-2;
        double ly = ny * ((radius + middle_radius) / 2+1) + y-2;
        switch ((i+2)%4)
        {
        case 0:
            dp.drawBitmap(round(lx), round(ly), epd_bitmap_north, 5, 5, fg_color);
            break;
        case 1:
            dp.drawBitmap(round(lx-0.5), round(ly), epd_bitmap_east, 4, 5, fg_color);
            break;
        case 2:
            dp.drawBitmap(round(lx), round(ly), epd_bitmap_south, 5, 5, fg_color);
            break;
        case 3:
            dp.drawBitmap(round(lx), round(ly), epd_bitmap_west, 5, 5, fg_color);
            break;
        }
        double temp = nx;
        nx = -ny;
        ny = temp;
        temp = n45x;
        n45x = -n45y;
        n45y = temp;
    }
}