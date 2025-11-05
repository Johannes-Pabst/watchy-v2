#pragma once

#include <Arduino.h>

// 'arrUp', 3x2px
const unsigned char epd_bitmap_arrUp [] PROGMEM = {
	0x40, 0xe0
};
// 'free', 2x2px
const unsigned char epd_bitmap_free [] PROGMEM = {
	0xc0, 0x80
};
// 'wDay', 2x2px
const unsigned char epd_bitmap_wDay [] PROGMEM = {
	0xc0, 0x40
};