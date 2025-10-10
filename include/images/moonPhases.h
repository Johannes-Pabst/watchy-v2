#pragma once

#include <Arduino.h>

// 'shrinking_moon', 7x7px
const unsigned char epd_bitmap_shrinking_moon [] PROGMEM = {
	0x38, 0x74, 0xe2, 0xe2, 0xe2, 0x74, 0x38
};
// 'new_moon', 7x7px
const unsigned char epd_bitmap_new_moon [] PROGMEM = {
	0x38, 0x44, 0x82, 0x82, 0x82, 0x44, 0x38
};
// 'growing_moon', 7x7px
const unsigned char epd_bitmap_growing_moon [] PROGMEM = {
	0x38, 0x5c, 0x8e, 0x8e, 0x8e, 0x5c, 0x38
};
// 'full_moon', 7x7px
const unsigned char epd_bitmap_full_moon [] PROGMEM = {
	0x38, 0x7c, 0xfe, 0xfe, 0xfe, 0x7c, 0x38
};