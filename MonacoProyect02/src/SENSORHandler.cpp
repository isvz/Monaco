#include "SENSORHandler.h"
#include <Arduino.h>
// Definición de pines y variables globales
const int oneWireBus1 = 26;  // El pin donde está conectado el primer sensor DS18B20
const int oneWireBus2 = 27;  // El pin donde está conectado el segundo sensor DS18B20
const int voltagePin = 35;  // Pin analógico para medir el voltaje de la batería 1 (ACS712T)
const int currentPin = 25;  // Pin analógico para medir la corriente de la batería 1 (L01Z200S05)

// Definiciones específicas para los sensores
const float ACS712T_VCC = 5.0;        // Voltaje de alimentación del ACS712T
const float ACS712T_OFFSET = ACS712T_VCC / 2; // Offset del ACS712T sin corriente
const float ACS712T_SENSITIVITY = 0.185;  // Sensibilidad del ACS712T (V/A) para 5A

const float HALL_VCC = 5.0;          // Voltaje de alimentación del sensor Hall
const float HALL_OFFSET = HALL_VCC / 2; // Offset del sensor Hall sin corriente
const float HALL_SENSITIVITY = 0.02;  // Sensibilidad del L01Z200S05 (V/A) para 200A

OneWire oneWire1(oneWireBus1);
OneWire oneWire2(oneWireBus2);

DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);

float battery1Temperature = 0.0;
float battery2Temperature = 0.0;
float battery1Voltage = 0.0;
float battery1Current = 0.0;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 1000; // Intervalo de actualización de 1 segundo

void setupSensors() {
  sensors1.begin();
  sensors2.begin();
  pinMode(voltagePin, INPUT);
  pinMode(currentPin, INPUT);
}

void loopSensors() {
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentTime;
    bool contactorState2 = LOW;
    // Leer temperaturas
    sensors1.requestTemperatures();
    sensors2.requestTemperatures();

    battery1Temperature = sensors1.getTempCByIndex(0);
    battery2Temperature = sensors2.getTempCByIndex(0);

    Serial.print("Battery 1 Temperature: ");
    Serial.print(battery1Temperature);
    Serial.println(" C");

    Serial.print("Battery 2 Temperature: ");
    Serial.print(battery2Temperature);
    Serial.println(" C");
    
    // Leer voltaje (ACS712T)
    int rawVoltage = analogRead(voltagePin);
    float voltage = (rawVoltage / 1024.0) * ACS712T_VCC;
    battery1Voltage = (voltage - ACS712T_OFFSET) / ACS712T_SENSITIVITY;

    Serial.print("Battery 1 Voltage: ");
    Serial.print(battery1Voltage);
    Serial.println(" V");

    // Leer corriente (L01Z200S05)
    int rawCurrent = analogRead(currentPin);
    float current = (rawCurrent / 1024.0) * HALL_VCC;
    battery1Current = (current - HALL_OFFSET) / HALL_SENSITIVITY;

    Serial.print("Battery 1 Current: ");
    Serial.print(battery1Current);
    Serial.println(" A");
  }
}
