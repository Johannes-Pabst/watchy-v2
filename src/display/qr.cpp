#include "display/qr.h"
#include <Arduino.h>
#include <qrcodegen.h>
#include "hardware/display.h"
#include <GxEPD2_BW.h>
#include "display/alignment.h"

static uint8_t qrcode[qrcodegen_BUFFER_LEN_FOR_VERSION(40)];
static uint8_t tempBuffer[qrcodegen_BUFFER_LEN_FOR_VERSION(40)];

void drawQr(String data, int px, int py, int width, int height, uint8_t alignment, qrcodegen_Ecc ecl)
{
    int minVersion = 1;
    // size=17+version*4
    // version=(size-17)/4
    int maxVersion = min(40,(min(width,height)-17)/4);
    qrcodegen_encodeText(data.c_str(), tempBuffer, qrcode, ecl, minVersion, maxVersion, qrcodegen_Mask_AUTO, false);
    int size = qrcodegen_getSize(qrcode);
    int size_mult=min(width, height)/size;
    uint8_t x_dir;
    uint8_t y_dir;
    splitAlignment(alignment, x_dir, y_dir);
    int16_t x_overflow = width - size*size_mult;
    int16_t x_offset;
    if (x_dir == TD1_START)
    {
        x_offset = 0;
    }else if (x_dir == TD1_CENTER)
    {
        x_offset = x_overflow / 2;
    }else if (x_dir == TD1_END)
    {
        x_offset = x_overflow;
    }
    int16_t y_overflow = height - size*size_mult;
    int16_t y_offset;
    if (y_dir == TD1_START)
    {
        y_offset = 0;
    }else if (y_dir == TD1_CENTER)
    {
        y_offset = y_overflow / 2;
    }else if (y_dir == TD1_END)
    {
        y_offset = y_overflow;
    }
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            bool module = qrcodegen_getModule(qrcode, x, y);
            dp.fillRect(px+x*size_mult+x_offset, py+y*size_mult+y_offset, size_mult, size_mult, module?GxEPD_BLACK:GxEPD_WHITE);
        }
    }
}