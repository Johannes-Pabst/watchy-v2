#pragma once
#include <Arduino.h>
#include <Ephemeris.hpp>

void clearSSOGraphCache();
void SSOGraphWidget(int16_t x, int16_t y, int16_t width, int16_t height, SolarSystemObjectIndex obj, uint8_t text_pos);
String doubleToTimeString(double time);