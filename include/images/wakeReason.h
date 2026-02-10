#pragma once

#include <Arduino.h>

// 'wake_symbol_back', 7x7px
const unsigned char epd_bitmap_wake_symbol_back [] PROGMEM = {
    0xf8, 0xc0, 0xa0, 0x90, 0x88, 0x04, 0x02
};
// 'wake_symbol_up', 7x7px
const unsigned char epd_bitmap_wake_symbol_up [] PROGMEM = {
    0x3e, 0x06, 0x0a, 0x12, 0x22, 0x40, 0x80
};
// 'wake_symbol_menu', 7x7px
const unsigned char epd_bitmap_wake_symbol_menu [] PROGMEM = {
    0x02, 0x04, 0x88, 0x90, 0xa0, 0xc0, 0xf8
};
// 'wake_symbol_down', 7x7px
const unsigned char epd_bitmap_wake_symbol_down [] PROGMEM = {
    0x80, 0x40, 0x22, 0x12, 0x0a, 0x06, 0x3e
};
// 'wake_symbol_reset', 7x7px
const unsigned char epd_bitmap_wake_symbol_reset [] PROGMEM = {
    0x10, 0x54, 0x92, 0x92, 0x82, 0x44, 0x38
};
// 'wake_symbol_rtc', 7x7px
const unsigned char epd_bitmap_wake_symbol_rtc [] PROGMEM = {
    0x38, 0x54, 0x92, 0x9a, 0x82, 0x44, 0x38
};