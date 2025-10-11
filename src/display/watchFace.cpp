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
#include "math/sky.h"
#include "images/tinyFont.h"

void watchFace(){
    dp.setFullWindow();
    dp.fillScreen(bg_color);

    measureFn(hourMinuteWidget, (0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT,&tinyFont,6));
    measureFn(batteryWidget, (0,0));
    measureFn(temperatureWidget, (0,24));
    measureFn(moonRenderWidget, (10, 150, 7));
    measureFn(moonPhasesWidget, (30, 0));
    measureFn(SSOGraphWidget, (50, 0,28+6+50,38, EarthsMoon, TD1_START));
    measureFn(SSOGraphWidget, (50, 38,28+6+50,38, Sun, TD1_END));
    measureFn(compassWidget, (50+19+(28+6+50), 19, 19, EarthsMoon));
    measureFn(compassWidget, (50+19+(28+6+50), 19+38, 19, Sun));
    measureFn(accellerationWidget, (0, 24+16));
    drawPerformanceWidget(50+19+(28+6+50)+19, 0);

    // drawQr("https://smart-watch.online-coding.de/",0,DISPLAY_HEIGHT/2, DISPLAY_WIDTH, DISPLAY_HEIGHT/2, TD_CENTER_CENTER);
    
    dp.display(esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_EXT0);
}