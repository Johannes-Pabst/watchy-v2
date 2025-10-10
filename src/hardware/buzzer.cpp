#include "hardware/buzzer.h"
#include <Arduino.h>

void buzz(int ms){
    digitalWrite(VIB_MOTOR_PIN, HIGH);
    delay(ms);
    digitalWrite(VIB_MOTOR_PIN, LOW);
}