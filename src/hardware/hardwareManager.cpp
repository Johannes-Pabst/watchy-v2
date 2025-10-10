#include "hardware/hardwareManager.h"
#include "hardware/display.h"
#include "hardware/rtc.h"
#include "hardware/bma.h"
#include "hardware/fs.h"
#include "math/config.h"
#include <Wire.h>
#include <Arduino.h>

void initHardware()
{
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    pinMode(BACK_BTN_PIN, INPUT);
    pinMode(UP_BTN_PIN, INPUT);
    pinMode(DOWN_BTN_PIN, INPUT);
    pinMode(MENU_BTN_PIN, INPUT);
    pinMode(ACC_INT_1_PIN, INPUT);
    pinMode(ACC_INT_2_PIN, INPUT);
    pinMode(RTC_INT_PIN, INPUT);
    initSensor();
    initRTC();
    initDisplay();
    initFs();
    loadConfig();
    esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_ALL);
}
void hibernateHardware()
{
    hibernate_display();
    esp_deep_sleep_start();
}
void shutdownHardware()
{
    shutdownSensor();
    hibernateHardware();
}