#include "display/unixTimestampWidget.h"
#include "display/text.h"
#include "display/alignment.h"
#include "math/time.h"

void unixTimestampWidget(int x, int y){
    SimpleTime now = simpleNowUTC();
    long timestamp = getUnixTimestamp(now);
    textBox(String(timestamp), x, y, 200, 8, TD_TOP_LEFT, nullptr, 1, fg_color);
}