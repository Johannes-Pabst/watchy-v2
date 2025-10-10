#include "hardware/rtc.h"
#include <WatchyRTC.h>

WatchyRTC RTC;

void initRTC() {
    RTC.init();
    RTC.clearAlarm();
}
void enableMinutelyWakeup()
{
    RTC.rtc_ds.squareWave(DS3232RTC::SQWAVE_NONE); // disable square wave output
    RTC.rtc_ds.setAlarm(DS3232RTC::ALM2_EVERY_MINUTE, 0, 0, 0,
                        0);                              // alarm wakes up Watchy every minute
    RTC.rtc_ds.alarmInterrupt(DS3232RTC::ALARM_2, true); // enable alarm interrupt
    esp_sleep_enable_ext0_wakeup((gpio_num_t)RTC_INT_PIN,
                                 0); // enable deep sleep wake on RTC interrupt
}