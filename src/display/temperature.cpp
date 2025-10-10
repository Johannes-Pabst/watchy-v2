#include "display/temperature.h"
#include "images/temperature.h"
#include "display/text.h"
#include "hardware/bma.h"
#include "hardware/display.h"
#include "display/alignment.h"
#include "hardware/bma.h"

void temperatureWidget(int16_t x, int16_t y){
    float celsius=getTemp();
    float fahrenheit=celsius*1.8+32;
    dp.drawBitmap(x,y,epd_bitmap_thermometer,5, 15, fg_color);
    float minc=20;
    float maxc=50;
    int h=max(0.0f,min(10.0f,round((celsius-minc)/(maxc-minc)*10)));
    dp.drawRect(x+2, y+1+10-h, 1, h, fg_color);
    textBox(String(int(round(celsius)))+"\n"+String(int(round(fahrenheit))),x+6,y,12,16,TD_TOP_LEFT);
    dp.drawBitmap(x+18,y,epd_bitmap_degrees,4, 4, fg_color);
    dp.drawBitmap(x+18,y+8,epd_bitmap_degrees,4, 4, fg_color);
    textBox("C\nF",x+23,y,12,16,TD_TOP_LEFT);
}