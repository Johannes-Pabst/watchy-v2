#include "display/dateWidget.h"
#include "display/text.h"
#include "display/alignment.h"
#include "hardware/display.h"
#include "math/time.h"
#include "math/timeTable.h"
#include "images/tinyFont.h"
#include "images/dateWidget.h"

// 70 * 20
void dateWidget(int16_t x, int16_t y)
{
    SimpleTime t = simpleNow();
    const char *daysOfWeek="MoTuWeThFrSaSu";
    int daysBevore=1;
    int daysAfter=5;
    t.day -= daysBevore;
    t.wday -= daysBevore;
    for(int i=0; i<daysBevore + daysAfter +1; i++){
        fixTime(t);
        if(t.wday==0||i==0){
            int wnum=getWeekNumber(t);
            textBox(String(wnum)+"\n"+String(t.day)+"\n"+String(daysOfWeek[t.wday*2])+String(daysOfWeek[t.wday*2+1]), x+i*10, y, 8+2, 18, TD_TOP_CENTER, &tinyFont);
        }else{
            textBox(String(t.day)+"\n"+String(daysOfWeek[t.wday*2])+String(daysOfWeek[t.wday*2+1]), x+i*10, y+6, 8+2, 12, TD_TOP_CENTER, &tinyFont);
        }
        if(isSchoolDay(t)){
            dp.drawBitmap(x+i*10, y+18, epd_bitmap_free, 2, 2, fg_color);
        }
        if(i==daysBevore){
            dp.drawBitmap(x+i*10+3, y+18, epd_bitmap_arrUp, 3, 2, fg_color);
        }
        if(t.wday>=5){
            dp.drawBitmap(x+i*10+7, y+18, epd_bitmap_wDay, 2, 2, fg_color);
        }
        t.day +=1;
        t.wday +=1;
    }
}