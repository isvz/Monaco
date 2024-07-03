#pragma once

#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include <OneWire.h>
#include <DallasTemperature.h>

void setupSensors();
void loopSensors();

extern float battery1Temperature;
extern float battery2Temperature;
extern float battery1Voltage;
extern float battery1Current;

#endif