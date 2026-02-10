#include "display/wakeReasonWidget.h"
#include "hardware/display.h"
#include "images/wakeReason.h"
#include "hardware/rtc.h"

// 7 * 7
void wakeReasonWidget(uint16_t x, uint16_t y){
    if(reason != ESP_SLEEP_WAKEUP_EXT0 && reason != ESP_SLEEP_WAKEUP_EXT1){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_reset,7,7,fg_color);
    }
    if(reason==ESP_SLEEP_WAKEUP_EXT0){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_rtc,7,7,fg_color);
    }
    if((reason==ESP_SLEEP_WAKEUP_EXT1)&&((pins&BACK_BTN_MASK)>0)){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_back,7,7,fg_color);
    }
    if((reason==ESP_SLEEP_WAKEUP_EXT1)&&((pins&UP_BTN_MASK)>0)){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_up,7,7,fg_color);
    }
    if((reason==ESP_SLEEP_WAKEUP_EXT1)&&((pins&MENU_BTN_MASK)>0)){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_menu,7,7,fg_color);
    }
    if((reason==ESP_SLEEP_WAKEUP_EXT1)&&((pins&DOWN_BTN_MASK)>0)){
        dp.drawBitmap(x,y,epd_bitmap_wake_symbol_down,7,7,fg_color);
    }
}