#pragma once

#include <Arduino.h>

// 'north', 5x5px
const unsigned char epd_bitmap_north [] PROGMEM = {
	0x88, 0xc8, 0xa8, 0x98, 0x88
};
// 'south', 5x5px
const unsigned char epd_bitmap_south [] PROGMEM = {
	0x78, 0x80, 0x70, 0x08, 0xf0
};
// 'west', 5x5px
const unsigned char epd_bitmap_west [] PROGMEM = {
	0xa8, 0xa8, 0xa8, 0xa8, 0x50
};
// 'east', 4x5px
const unsigned char epd_bitmap_east [] PROGMEM = {
	0xf0, 0x80, 0xe0, 0x80, 0xf0
};