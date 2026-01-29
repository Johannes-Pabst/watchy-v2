#pragma once

#include <Arduino.h>
#include <GxEPD2_BW.h>

void hourMinuteWidget(int16_t x, int16_t y, int16_t width, int16_t height, const GFXfont *f = (const GFXfont *)__null, uint8_t fontSize = 1, boolean utc=false);