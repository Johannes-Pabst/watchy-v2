#include <WatchyRTC.h>
#include <bma.h>

extern RTC_DATA_ATTR BMA423 sensor;

void configSensor();
void initSensor();
uint32_t getSteps();
float getTemp();
void resetSteps();
bool getAcc(Accel &acc);
void enablePickupWakeup();
void shutdownSensor();