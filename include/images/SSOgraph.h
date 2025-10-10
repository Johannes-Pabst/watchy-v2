#pragma once
#include <Arduino.h>

// 'sun', 7x7px
const unsigned char epd_bitmap_sun [] PROGMEM = {
	0x10, 0x54, 0x38, 0xfe, 0x38, 0x54, 0x10
};
// 'arr_right', 2x3px
const unsigned char epd_bitmap_arr_right [] PROGMEM = {
	0x80, 0xc0, 0x80
};
// 'arr_left', 2x3px
const unsigned char epd_bitmap_arr_left [] PROGMEM = {
	0x40, 0xc0, 0x40
};
// 'arr_ud', 5x7px
const unsigned char epd_bitmap_arr_ud [] PROGMEM = {
	0x20, 0x60, 0xa0, 0x20, 0x28, 0x30, 0x20
};
// 'arr_du', 5x7px
const unsigned char epd_bitmap_arr_du [] PROGMEM = {
	0x20, 0x30, 0x28, 0x20, 0xa0, 0x60, 0x20
};