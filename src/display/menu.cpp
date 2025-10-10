#include "display/menu.h"
#include "display/text.h"
#include "hardware/buzzer.h"
#include "hardware/display.h"
#include "images/menuIcons.h"
#include <Arduino.h>
#include "display/alignment.h"
#include <vector>

int drawMenu(std::vector<String> options)
{
    int selected = 0;
    bool first = true;
    bool up_held=false;
    bool down_held=false;
    while (true)
    {
        dp.setFullWindow();
        dp.fillScreen(bg_color);

        dp.drawBitmap(1, 1, epd_bitmap_back, 7, 9, fg_color);
        dp.drawBitmap(DISPLAY_WIDTH - 7 - 1, 1, epd_bitmap_up, 7, 9, fg_color);
        dp.drawBitmap(1, DISPLAY_HEIGHT - 5 - 1, epd_bitmap_ok, 7, 5, fg_color);
        dp.drawBitmap(DISPLAY_WIDTH - 7 - 1, DISPLAY_HEIGHT - 9 - 1, epd_bitmap_down, 7, 9, fg_color);

        int offset_x_const = 8;
        int offset_y_const = 10;
        int offset_y = offset_y_const;
        for (int i = 0; i < options.size(); i++)
        {
            int height = 0;
            uint16_t color;
            if (i == selected)
            {
                color = bg_color;
                dp.fillRect(offset_x_const, offset_y, DISPLAY_WIDTH - offset_x_const * 2, DISPLAY_HEIGHT - offset_y - offset_y_const, fg_color);
            }
            else
            {
                color = fg_color;
            }
            textBox(options[i], offset_x_const, offset_y+1, DISPLAY_WIDTH - offset_x_const * 2, DISPLAY_HEIGHT - offset_y - offset_y_const-1, TD_TOP_CENTER, __null, 1, color, nullptr, &height);
            offset_y += height + 2;
            if (i == selected)
            {
                color = bg_color;
                dp.fillRect(offset_x_const, offset_y, DISPLAY_WIDTH - offset_x_const * 2, DISPLAY_HEIGHT - offset_y - offset_y_const, bg_color);
            }
        }

        dp.display(!first);
        first = false;
        while(true){
            if(digitalRead(BACK_BTN_PIN)==HIGH){
                return -1;
            }
            if(digitalRead(UP_BTN_PIN)==HIGH){
                selected--;
                selected+=options.size();
                selected%=options.size();
                if(up_held){
                    buzz(20);
                    delay(150-50);
                    continue;
                }
                up_held=true;
                break;
            }else if(up_held){
                up_held=false;
                break;
            }
            if(digitalRead(DOWN_BTN_PIN)==HIGH){
                selected++;
                selected%=options.size();
                if(down_held){
                    buzz(20);
                    delay(150-50);
                    continue;
                }
                down_held=true;
                break;
            }else if(down_held){
                down_held=false;
                break;
            }
            if(digitalRead(MENU_BTN_PIN)==HIGH){
                return selected;
            }
            delay(50);
        }
    }
}