#include "hardware/battery.h"
#include <Arduino.h>
#include <WatchyRTC.h>
#include "hardware/rtc.h"
#include "hardware/hardwareManager.h"
#include "hardware/display.h"
#include "hardware/buttons.h"
#include "display/text.h"
#include "display/hourMinute.h"
#include "display/alignment.h"
#include "images/tinyFont.h"

float getBatteryVoltage()
{
    return analogReadMilliVolts(BATT_ADC_PIN) / 1000.0f * 2.0f;
}
float getBatteryPercentage()
{
    float voltage = getBatteryVoltage();
    const float voltages[] = {4.2, 4.15, 4.11, 4.08, 4.02, 3.98, 3.95, 3.91, 3.87, 3.85, 3.84, 3.82, 3.80, 3.79, 3.77, 3.75, 3.73, 3.71, 3.69, 3.61, 3.27};
    const float percentages[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
    int id1 = 0;
    while (voltages[id1] > voltage)
    {
        id1++;
    }
    int id2 = id1 + 1;
    float diffv = voltages[id2] - voltages[id1];
    float diffp = percentages[id2] - percentages[id1];
    float percentage = (voltage - voltages[id1]) / diffv * diffp + percentages[id1];
    return percentage;
}
void checkEmergencyShutdown()
{
    if (getBatteryPercentage() < 5.0f)
    {
        dp.setFullWindow();
        dp.fillScreen(bg_color);
        textBox("Battery critically low:\n" + String(getBatteryVoltage()) + "V\nShutting down to\nprotect battery.", 10, 10, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 20, TD_CENTER_CENTER, __null, 2, fg_color);
        dp.display(false);
        enableWakeupOnButtonPress();
        shutdownHardware();
    }
}
void autoShutdown()
{
    dp.setFullWindow();
    dp.fillScreen(bg_color);
    textBox("No movement.\nshutting down\nto save battery.", 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT-100, TD_CENTER_CENTER, __null, 2, fg_color);
    hourMinuteWidget(0, 50, DISPLAY_WIDTH, DISPLAY_HEIGHT-50, &tinyFont, 6);
    dp.display(false);
    enableWakeupOnButtonPress();
    shutdownHardware();
}