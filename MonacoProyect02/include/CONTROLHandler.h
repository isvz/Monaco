#pragma once
#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

#include <Arduino.h>

void setupControl();
void loopControl();
bool debounce(int pin);

#endif
