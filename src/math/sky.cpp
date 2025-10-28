#include "math/sky.h"
#include "math/time.h"
#include "math/config.h"
#include "watchyV2config.h"
#include <Ephemeris.h>

SolarSystemObject solarSystemObjects[10]={};
bool sky_initialized[10]={};

SolarSystemObject* getCurrent(SolarSystemObjectIndex obj){
    resetEphemeris();
    if(!sky_initialized[obj]){
        SimpleTime t=simpleNowUTC();
        solarSystemObjects[obj]=calculatePos(obj, &t);
        sky_initialized[obj]=true;
    }
    return &solarSystemObjects[obj];
}
SolarSystemObject calculatePos(SolarSystemObjectIndex obj, SimpleTime *t){
    return Ephemeris::solarSystemObjectAtDateAndTime(obj,
        t->day, t->month, t->year,
        t->hour, t->minute, t->second);
}

double latitude;
double longitude;
double riseSetAltitude;
bool sky_config_initialized=false;
void resetEphemeris(){
    if(!sky_config_initialized){
        JSONVar cfg = parseCategory("Location", config);
        latitude = parseFloat("Latitude", cfg);
        longitude = parseFloat("Longitude", cfg);
        if(latitude==0.0){
            latitude=40.0;
        }
        if(longitude==0.0){
            longitude=-40.0;
        }
        riseSetAltitude = parseFloat("rise and set elevation", cfg);
        sky_config_initialized=true;
    }
    Ephemeris::setLocationOnEarth(latitude, longitude);
    Ephemeris::setAltitude(riseSetAltitude);
}