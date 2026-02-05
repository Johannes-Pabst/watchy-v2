#include "display/moonRender.h"
#include <Arduino.h>
#include <Ephemeris.hpp>
#include "hardware/rtc.h"
#include "hardware/display.h"
#include "display/text.h"
#include "math/vec3.h"
#include "math/sky.h"

// circle  at center (x, y) with radius
void moonRenderWidget(int16_t x, int16_t y, int16_t radius)
{
    SolarSystemObject *moon = getCurrent(EarthsMoon);
    SolarSystemObject *sun = getCurrent(Sun);
    float moonx = cos(DEG_TO_RAD * moon->horiCoordinates.alt) * sin(DEG_TO_RAD * moon->horiCoordinates.azi);
    float moony = sin(DEG_TO_RAD * moon->horiCoordinates.alt);
    float moonz = cos(DEG_TO_RAD * moon->horiCoordinates.alt) * cos(DEG_TO_RAD * moon->horiCoordinates.azi);

    float sunx = cos(DEG_TO_RAD * sun->horiCoordinates.alt) * sin(DEG_TO_RAD * sun->horiCoordinates.azi);
    float suny = sin(DEG_TO_RAD * sun->horiCoordinates.alt);
    float sunz = cos(DEG_TO_RAD * sun->horiCoordinates.alt) * cos(DEG_TO_RAD * sun->horiCoordinates.azi);

    /*
    (moon*y) -> x
    (moon*y)*moon -> -(!)y
    moon -> z
    */
    
    Vec3 vmoon={moonx, moony, moonz};
    Vec3 v1=vmoon.cross({0,1,0}).normalize();
    Vec3 v2=v1.cross(vmoon).normalize();
    Vec3 v3=vmoon.normalize();

    float x1 = v1.x;
    float y1 = v1.y;
    float z1 = v1.z;

    float x2 = v2.x;
    float y2 = v2.y;
    float z2 = v2.z;

    float x3 = v3.x;
    float y3 = v3.y;
    float z3 = v3.z;

    float x4 = sunx;
    float y4 = suny;
    float z4 = sunz;

    float divisor = (-x1 * y2 * z3 + x1 * y3 * z2 + x2 * y1 * z3 - x2 * y3 * z1 - x3 * y1 * z2 + x3 * y2 * z1);
    float sunxnew = -(x2 * y3 * z4 - x2 * y4 * z3 - x3 * y2 * z4 + x3 * y4 * z2 + x4 * y2 * z3 - x4 * y3 * z2) / divisor;
    float sunynew = -(-x1 * y3 * z4 + x1 * y4 * z3 + x3 * y1 * z4 - x3 * y4 * z1 - x4 * y1 * z3 + x4 * y3 * z1) / divisor;
    float sunznew = -(x1 * y2 * z4 - x1 * y4 * z2 - x2 * y1 * z4 + x2 * y4 * z1 + x4 * y1 * z2 - x4 * y2 * z1) / divisor;

    Vec3 dirsunnew={-sunxnew,-sunynew,-sunznew};

    dp.drawCircle(x,y,radius,fg_color);

    for(int ix=x-radius;ix<=x+radius;ix++){
        for(int iy=y-radius;iy<=y+radius;iy++){
            float rx=ix-x;
            float ry=iy-y;
            rx/=radius;
            ry/=radius;
            float len=sqrt(rx*rx+ry*ry);
            if(len<1){
                float z=sqrt(1-len);
                Vec3 vnorm={rx,ry,z};
                float dotp=vnorm.dot(dirsunnew);
                if(dotp>0){
                    dp.drawPixel(ix, iy, fg_color);
                }
            }
        }
    }
    // Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(moon.rise, 2), &hours, &minutes, &seconds);
    // String rise = String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
    // Ephemeris::floatingHoursToHoursMinutesSeconds(Ephemeris::floatingHoursWithUTCOffset(moon.set, 2), &hours, &minutes, &seconds);
    // String set = String(hours) + "h " + String(minutes) + "m " + String(seconds) + "s";
    // textBox("rise: " + rise + "\nset: " + set, x, y, 150, 150, TD_TOP_LEFT);
}