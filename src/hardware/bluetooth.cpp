#include "hardware/bluetooth.h"
// #include "hardware/bma.h"
// #include <BleMouse.h>
// #include <Arduino.h>

// BleMouse bleMouse("ESP32 Mouse", "Espressif", 100);
// void bluetoothMouseSetup()
// {
//     bleMouse.begin();

//     while (digitalRead(BACK_BTN_PIN) == LOW)
//     {
//         if (bleMouse.isConnected())
//         {
//             Accel acc;
//             if (getAcc(acc))
//             {
//                 bleMouse.move(acc.x, acc.y);
//             }
//             Serial.println("Moving mouse...");
//             delay(30);
//         }
//     }
// }