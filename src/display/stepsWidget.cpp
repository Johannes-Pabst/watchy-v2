#include "display/stepsWidget.h"
#include "hardware/bma.h"
#include "hardware/display.h"
#include "display/text.h"
#include "display/alignment.h"
#include "math/padUtils.h"
#include "images/performance.h"

void stepsWidget(int16_t x, int16_t y){
    int steps=getSteps();
    textBox(padLeft(String(steps), 5), x, y, 5*6, 8, TD_TOP_LEFT);
    dp.drawBitmap(x+5*6, y, epd_bitmap_stepsWidget, 7, 7, fg_color);
}