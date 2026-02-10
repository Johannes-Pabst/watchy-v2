#include "hardware/rtc.h"
#include <WatchyRTC.h>

WatchyRTC RTC;
int reason=esp_sleep_get_wakeup_cause();
uint64_t pins=esp_sleep_get_ext1_wakeup_status();

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