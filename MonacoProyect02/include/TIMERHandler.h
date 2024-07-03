#pragma once

#ifndef TIMERHANDLER_H
#define TIMERHANDLER_H

#include <Arduino.h>

extern bool timer_fired;
extern unsigned long lastButtonPressTime;

void setupTimer(int frequency);
bool debounce(int pin);

#endif
