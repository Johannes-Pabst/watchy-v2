#pragma once

#include <Arduino.h>
#include <Display.h>
#include <GxEPD2_BW.h>

extern GxEPD2_BW<WatchyDisplay, WatchyDisplay::HEIGHT> dp;
extern uint16_t fg_color;
extern uint16_t bg_color;

void initDisplay();
void darkMode(bool darkMode = true);
void hibernate_display();
void displayMessage(String message);