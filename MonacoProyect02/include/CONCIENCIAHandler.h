
#pragma once
#ifndef CONCIENCIAHANDLER_H
#define CONCIENCIAHANDLER_H
#include <Arduino.h>

void setupSensing();
void loopSensing();

extern bool isUnconscious;
extern bool isSensorEnabled;

#endif
