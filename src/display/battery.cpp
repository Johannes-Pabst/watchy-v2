#include "display/battery.h"
#include <Arduino.h>
#include "hardware/battery.h"
#include "hardware/display.h"
#include "display/text.h"
#include "images/battery.h"
#include "display/alignment.h"

void batteryWidget(int16_t x, int16_t y){
    float percentage=getBatteryPercentage();
    if(percentage>15){
        dp.drawBitmap(x,y,epd_bitmap_battery,29, 7, fg_color);
    }else{
        dp.drawBitmap(x,y,epd_bitmap_battery_empty,29, 7, fg_color);
    }
    dp.fillRect(x+7,y+1,min(int(floor(16.0*percentage/100.0)),15),5,fg_color);
    String pcstring=String(percentage,1);
    if (pcstring.length()>4){
        pcstring=String(int(round(percentage)));
    }
    String text=String(getBatteryVoltage(),2)+"V\n"+pcstring+"%";
    textBox(text,x,y+8,100,100,TD_TOP_LEFT);
}