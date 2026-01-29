#include "display/worldMapWidget.h"
#include "images/worldmap.h"
#include "hardware/display.h"
#include "math/sky.h"

#include <Arduino.h>

void worldMapWidget(uint16_t x, uint16_t y){
    uint16_t width=52;
    uint16_t height=40;
    SolarSystemObject *sun=getCurrent(Sun);
    double slat=-sun->equaCoordinates.dec/180*PI;
    double slon=sun->equaCoordinates.ra/12*PI;
    double sy=sin(slat);
    double sx=cos(slat)*sin(slon);
    double sz=cos(slat)*cos(slon);
    double max_x = 2.0 * PI / sqrt(2.0);
    double max_y = (1.0 + sqrt(2.0) / 2.0) * tan(PI / 4.0) * 2.0;
    double minlat=1000;
    double maxlat=-1000;
    double minlon=1000;
    double maxlon=-1000;
    for (int ix = 0; ix < width; ix++)
    {
        for (int iy = 0; iy < height; iy++)
        {
            double lon=(((double)ix)/width-0.5)*max_x*sqrt(2.0);
            double lat=2.0*atan(((((double)iy)/height-0.5)*max_y)/(1.0+sqrt(2.0)/2.0));
            double py=sin(lat);
            double px=cos(lat)*sin(lon);
            double pz=cos(lat)*cos(lon);
            double dot=sx*px+sy*py+sz*pz;
            if(ix%5==0&&iy%5==0){
                // Serial.println("seperator");
                // Serial.println(ix);
                // Serial.println(iy);
                // Serial.println(lon);
                // Serial.println(lat);
                // Serial.println(dot);
            }
            minlat=min(minlat, lat);
            minlon=min(minlon, lon);
            maxlat=max(maxlat, lat);
            maxlon=max(maxlon, lon);
            if(
                (((epd_bitmap_world_map[(ix+iy*width)/8]<<((ix+iy*width)%8))&0b10000000)>0)==
                (dot>0)){
                    dp.drawPixel(x+ix,y+iy,fg_color);
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