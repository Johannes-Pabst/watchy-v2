#include "hardware/buttons.h"
#include "hardware/battery.h"
#include <Arduino.h>

void enableWakeupOnButtonPress() {
        esp_sleep_enable_ext1_wakeup(
        BTN_PIN_MASK,
        ESP_EXT1_WAKEUP_ANY_HIGH
    );
}
int awaitButtonPress(int timeout_ms){
    int start=millis();
    while(timeout_ms==0||start+timeout_ms>millis()){
        if(digitalRead(BACK_BTN_PIN)==HIGH){
            return BACK_BTN_PIN;
        }
        if(digitalRead(UP_BTN_PIN)==HIGH){
            return UP_BTN_PIN;
        }
        if(digitalRead(DOWN_BTN_PIN)==HIGH){
            return DOWN_BTN_PIN;
        }
        if(digitalRead(MENU_BTN_PIN)==HIGH){
            return MENU_BTN_PIN;
        }
        checkEmergencyShutdown();
        delay(50);
    }
    return -1;
}