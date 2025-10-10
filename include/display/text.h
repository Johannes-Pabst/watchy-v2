#pragma once

#include <GxEPD2_BW.h>
#include <hardware/display.h>

bool textBox(String text, int16_t x, int16_t y, int16_t width, int16_t height, u_int8_t td, const GFXfont *f = (const GFXfont *)__null, uint8_t fontSize = 1, uint16_t color=fg_color, int *outw=nullptr, int *outh=nullptr);
uint8_t getXAdvance(const GFXfont *f, char c);
uint8_t getYAdvance(const GFXfont *f);
void splitAlignment(u_int8_t td, uint8_t &x_dir, uint8_t &y_dir);
