#include "display/hourMinute.h"
#include "display/text.h"
#include "hardware/rtc.h"
#include <Arduino.h>
#include "display/alignment.h"
#include "math/time.h"

void hourMinuteWidget(int16_t x, int16_t y, int16_t width, int16_t height, const GFXfont *f, uint8_t fontSize){
    SimpleTime t=simpleNow();
    textBox(String(t.hour)+':'+String(t.minute),x,y,width,height,TD_CENTER_CENTER, f, fontSize);
}