#pragma once

#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Definición de pines y dirección I2C
extern int lcdAddr;
extern int lcdColumns;
extern int lcdRows;
extern LiquidCrystal_PCF8574 lcd;

// Definición de pines para OneWire
#define ONE_WIRE_BUS1 33
#define ONE_WIRE_BUS2 26

void setupdisplay();
void updateValues(float temp_bateria1, float voltaje_B1, float corriente_B1, float temp_bateria2);

#endif // DISPLAY_H
