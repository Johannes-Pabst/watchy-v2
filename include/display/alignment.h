#pragma once

#include <Arduino.h>

#define TD_TOP_LEFT 0
#define TD_TOP_CENTER 1
#define TD_TOP_RIGHT 2
#define TD_CENTER_LEFT 3
#define TD_CENTER_CENTER 4
#define TD_CENTER_RIGHT 5
#define TD_BOTTOM_LEFT 6
#define TD_BOTTOM_CENTER 7
#define TD_BOTTOM_RIGHT 8

#define TD1_START 0
#define TD1_CENTER 1
#define TD1_END 2

void splitAlignment(u_int8_t td, uint8_t &x_dir, uint8_t &y_dir);