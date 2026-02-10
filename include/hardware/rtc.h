#pragma once

#include <WatchyRTC.h>

extern WatchyRTC RTC;
extern int reason;
extern uint64_t pins;

void initRTC();
void enableMinutelyWakeup();