#include "display/watchFace.h"
#include "hardware/display.h"
#include "display/hourMinute.h"
#include "display/battery.h"
#include "display/temperature.h"
#include "display/moonRender.h"
#include "display/moonPhases.h"
#include "display/SSOgraph.h"
#include "display/compass.h"
#include "display/qr.h"
#include "display/alignment.h"
#include "display/daysSince.h"
#include "display/performanceWidget.h"
#include "display/accelleration.h"
#include "display/stepsWidget.h"
#include "display/leapyear.h"
#include "display/nextLessonsWidget.h"
#include "display/unixTimestampWidget.h"
#include "display/timeTableProgressBar.h"
#include "display/dateWidget.h"
#include "display/worldMapWidget.h"
#include "display/wakeReasonWidget.h"
#include "math/sky.h"
#include "images/tinyFont.h"

void watchFace(){
    dp.setFullWindow();
    dp.fillScreen(bg_color);

    resetEphemeris();
    measureFn(hourMinuteWidget, (0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT-5*2,&tinyFont,6));
    measureFn(hourMinuteWidget, (0,0,6*5,DISPLAY_HEIGHT,nullptr,1, true));
    measureFn(batteryWidget, (0,0));
    measureFn(temperatureWidget, (0,24));
    measureFn(moonPhasesWidget, (30, 0));
    measureFn(wakeReasonWidget, (42, 40));
    measureFn(SSOGraphWidget, (50, 0,28+6+50,38, EarthsMoon, TD1_START));
    measureFn(SSOGraphWidget, (50, 38,28+6+50,38, Sun, TD1_END));
    measureFn(moonRenderWidget, (50+28+6+50-11+19*2, 100-6/2-5-1, 11));
    measureFn(compassWidget, (50+19+(28+6+50), 19, 19, EarthsMoon));
    measureFn(compassWidget, (50+19+(28+6+50), 19+38, 19, Sun));
    measureFn(accellerationWidget, (0, 24+16));
    measureFn(stepsWidget, (0, 24+16+24));
    measureFn(leapyearWidget, (0, 24+16+24+8, 5));
    measureFn(dateWidget, (0, 113-5));
    measureFn(unixTimestampWidget, (0, 149));
    measureFn(nextLessonsWidget, (0, 157, 4));
    measureFn(timeTableProgressBar, (0, 189, DISPLAY_WIDTH, 10));
    measureFn(worldMapWidget, (121, 113-5));
    drawPerformanceWidget(50+19+(28+6+50)+19+3, 0);
    
    dp.display(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0);
}
/*
TODO:
month,
year,
steps graph,
daily streak,
day progress,
last update,
auto-update,
weather,
battery graph,
wake reason,
procedural pattern,
quote,
drift estimator,
*/