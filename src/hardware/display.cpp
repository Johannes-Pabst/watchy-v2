#include "hardware/display.h"
#include <Display.h>
#include <GxEPD2_BW.h>

GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> dp(
    WatchyDisplay{});
uint16_t bg_color = GxEPD_BLACK;
uint16_t fg_color = GxEPD_WHITE;

void initDisplay(){
    dp.epd2.initWatchy();
    darkMode();
}
void darkMode(bool darkMode)
{
    if(darkMode) {
        bg_color = GxEPD_BLACK;
        fg_color = GxEPD_WHITE;
    }else{
        bg_color = GxEPD_WHITE;
        fg_color = GxEPD_BLACK;
    }
}
void hibernate_display()
{
    dp.epd2.hibernate();
}