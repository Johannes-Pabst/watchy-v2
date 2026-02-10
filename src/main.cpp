#include <Arduino.h>
#include "hardware/display.h"
#include <WatchyRTC.h>
#include "hardware/rtc.h"
#include "hardware/bma.h"
#include "hardware/buttons.h"
#include "hardware/buzzer.h"
#include "hardware/battery.h"
#include "hardware/hardwareManager.h"
#include "display/watchFace.h"
#include "menu/main.h"

void setup()
{
    Serial.begin(115200);
    Serial.flush();
    initHardware();
    if (reason != ESP_SLEEP_WAKEUP_EXT0 && reason != ESP_SLEEP_WAKEUP_EXT1)
    {
        buzz(100);
    }
    if(((reason==ESP_SLEEP_WAKEUP_EXT1)&&((pins&MENU_BTN_MASK)>0))||digitalRead(MENU_BTN_PIN)==HIGH){
        openMainMenu();
    }
    watchFace();
    checkEmergencyShutdown();
    if(reason==ESP_SLEEP_WAKEUP_EXT0&&shouldTurnOff()){
        autoShutdown();
    }
    enableMinutelyWakeup();
    enableWakeupOnButtonPress();
    hibernateHardware();
}

void loop()
{
}
/*
corrected by -166s in ~1.5 weeks =>~17s per day
*/