#pragma once

#include <Arduino.h>
#include <Ephemeris.hpp>
#include "math/time.h"

extern SolarSystemObject solarSystemObjects[10];

SolarSystemObject* getCurrent(SolarSystemObjectIndex obj);
SolarSystemObject calculatePos(SolarSystemObjectIndex obj, SimpleTime *t);
void resetEphemeris();