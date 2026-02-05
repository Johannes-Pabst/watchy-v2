#include "display/worldMapWidget.h"
#include "images/worldmap.h"
#include "hardware/display.h"
#include "math/sky.h"

#include <Ephemeris.hpp>
#include <Arduino.h>

#define T_WITH_JD(day, time) ((day - 2451545.0 + time) / 36525)
#define COSD(value) cos((value) * 0.0174532925)

// 52 * 40
void worldMapWidget(uint16_t x, uint16_t y)
{
    uint16_t width = 52;
    uint16_t height = 40;
    SimpleTime now = simpleNowUTC();
    SolarSystemObject *sun = getCurrent(Sun);

    FLOAT deltaNutation;
    JulianDay jd = Calendar::julianDayForDateAndTime(now.day, now.month, now.year, now.hour, now.minute, now.second);
    FLOAT T = T_WITH_JD(jd.day, jd.time);
    FLOAT epsilon = Ephemeris::obliquityAndNutationForT(T, NULL, &deltaNutation);

    FLOAT meanSideralTime = Ephemeris::meanGreenwichSiderealTimeAtJD(jd);

    // Apparent sideral time in floating hours
    FLOAT theta0 = meanSideralTime + (deltaNutation / 15 * COSD(epsilon)) / 3600;
    // Subsolar latitude
    double slat = -sun->equaCoordinates.dec / 180 * PI;

    // Greenwich hour angle (hours)
    double H = theta0 - sun->equaCoordinates.ra;

    // Normalize to [-12h, +12h]
    while (H < -12.0)
        H += 24.0;
    while (H > 12.0)
        H -= 24.0;

    // Subsolar longitude (degrees, east-positive)
    double slon = -15.0 * H;

    // Normalize to [-180°, +180°]
    while (slon < -180.0)
        slon += 360.0;
    while (slon > 180.0)
        slon -= 360.0;
    
    slon*=PI/180;

    double sy = sin(slat);
    double sx = cos(slat) * sin(slon);
    double sz = cos(slat) * cos(slon);
    double max_x = 2.0 * PI / sqrt(2.0);
    double max_y = (1.0 + sqrt(2.0) / 2.0) * tan(PI / 4.0) * 2.0;
    double minlat = 1000;
    double maxlat = -1000;
    double minlon = 1000;
    double maxlon = -1000;
    for (int ix = 0; ix < width; ix++)
    {
        for (int iy = 0; iy < height; iy++)
        {
            double lon = (((double)ix) / width - 0.5) * max_x * sqrt(2.0);
            double lat = 2.0 * atan(((((double)iy) / height - 0.5) * max_y) / (1.0 + sqrt(2.0) / 2.0));
            double py = sin(lat);
            double px = cos(lat) * sin(lon);
            double pz = cos(lat) * cos(lon);
            double dot = sx * px + sy * py + sz * pz;
            if (ix % 5 == 0 && iy % 5 == 0)
            {
                // Serial.println("seperator");
                // Serial.println(ix);
                // Serial.println(iy);
                // Serial.println(lon);
                // Serial.println(lat);
                // Serial.println(dot);
            }
            minlat = min(minlat, lat);
            minlon = min(minlon, lon);
            maxlat = max(maxlat, lat);
            maxlon = max(maxlon, lon);
            if (
                (((epd_bitmap_world_map[(ix + iy * width) / 8] << ((ix + iy * width) % 8)) & 0b10000000) > 0) ==
                (dot > 0))
            {
                dp.drawPixel(x + ix, y + iy, fg_color);
            }
        }
    }
    // Serial.println(slon);
    // Serial.println(slat);
    // Serial.println(minlat);
    // Serial.println(maxlat);
    // Serial.println(minlon);
    // Serial.println(maxlon);
}