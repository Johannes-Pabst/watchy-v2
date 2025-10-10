#include "display/daysSince.h"
#include "display/text.h"
#include "display/alignment.h"
#include "hardware/display.h"
#include "images/daysSince.h"
#include "images/tinyFont.h"
#include <Arduino.h>

void daysSinceWidget(int16_t x, int16_t y)
{
    String editable = "38";
    /*
    38 days since
    someone reset
      this sign
    */
    for (int i = 0; i < editable.length(); i++)
    {
        char c = editable.charAt(i);
        dp.drawBitmap(x + i * 8, y, epd_bitmap_editable_sign, 7, 10, fg_color);
        textBox(String(c), x + i * 8 + 2, y + 3, 4, 6, TD_TOP_LEFT, &tinyFont);
    }
    dp.setFont(__null);
    dp.setCursor(x + editable.length() * 8 - 2, y + 2);
    dp.print(" days since");
    textBox(" days since", x + editable.length() * 8 - 2, y + 2, 6*11, 8 * 1, TD_TOP_LEFT);
    textBox("someone reset\nthis sign", x, y + 11, 13 * 6, 8 * 2, TD_TOP_CENTER);
}