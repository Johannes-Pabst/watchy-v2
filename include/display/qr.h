#pragma once

#include <Arduino.h>
#include <qrcodegen.h>

void drawQr(String data, int px, int py, int width, int height, uint8_t alignment, qrcodegen_Ecc ecl=qrcodegen_Ecc_LOW);