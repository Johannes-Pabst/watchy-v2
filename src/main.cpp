#include <Arduino.h>
#include "hardware/display.h"
#include <WatchyRTC.h>
#include "hardware/rtc.h"
#include "hardware/bma.h"
#include "hardware/buttons.h"
#include "hardware/buzzer.h"
#include "hardware/hardwareManager.h"
#include "display/watchFace.h"
#include "menu/main.h"

void setup()
{
    Serial.begin(115200);
    Serial.flush();
    initHardware();
    int reason=esp_sleep_get_wakeup_cause();
    int pins=esp_sleep_get_ext1_wakeup_status();
    if (reason != ESP_SLEEP_WAKEUP_EXT0 && reason != ESP_SLEEP_WAKEUP_EXT1)
    {
        buzz(100);
    }
    if(reason==ESP_SLEEP_WAKEUP_EXT1&&(pins&MENU_BTN_MASK)){
        openMainMenu();
    }
    watchFace();
    enableMinutelyWakeup();
    enableWakeupOnButtonPress();
    hibernateHardware();
}

void loop()
{
}