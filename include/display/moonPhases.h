#pragma once
#include <Arduino.h>
#include "math/time.h"

void clearMoonPhaseCache();
void moonPhasesWidget(int16_t x, int16_t y);
float getMoonPhase(SimpleTime *t);
float getScore(float phase, int target);