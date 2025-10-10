#include "menu/main.h"
#include "display/menu.h"
#include <Arduino.h>
#include <vector>
#include "hardware/wifi.h"
#include "hardware/buttons.h"

#include <LittleFS.h>
#include "display/text.h"
#include "display/alignment.h"
#include "display/moonPhases.h"
#include "display/SSOgraph.h"
#include "hardware/bluetooth.h"

void openMainMenu(){
    switch(drawMenu(std::vector<String>{"open AP", "show options", "update RTC", "clear cache"})){
        case 0:
        setupAP();
        awaitButtonPress(0);
        stopAP();
        break;
        case 1:
        {
            File file=LittleFS.open("/config.json");
    
            dp.setFullWindow();
            dp.fillScreen(bg_color);
            textBox(file.readString(),0,0,200,200,TD_CENTER_CENTER);
            dp.display(false);
    
            Serial.println(file.readString());
            file.close();
            while(digitalRead(BACK_BTN_PIN)==LOW){
                delay(100);
            }
        }
        break;
        case 2:
        connectWifi();
        waitForConnection();
        updateRtc();
        disconnectWifi();
        break;
        case 3:
        clearMoonPhaseCache();
        clearSSOGraphCache();
        break;
        // case 3:
        // bluetoothMouseSetup();
        // break;
    }
}