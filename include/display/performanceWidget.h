#pragma once

#include <Arduino.h>
#include "images/performance.h"

#define measureFn(fnName, fnParams) {                \
    uint32_t start = millis();                       \
    fnName fnParams;                                 \
    uint32_t end = millis();                         \
    addPerformanceMeasurement(#fnName, end-start, epd_bitmap_##fnName);   \
}

void addPerformanceMeasurement(const char* name, uint32_t duration, const uint8_t *bitmap);
void drawPerformanceWidget(int16_t x, int16_t y);