#include <Arduino.h>
#include "CONCIENCIAHandler.h"
#include "CONTROLHandler.h"
#include "DISPLAYHandler.h"
#include "SENSORHandler.h"
#include "TIMERHandler.h"

void setup() {

  Serial.begin(9600); // Iniciar comunicación serial para debug
  setupSensing();
  setupControl();
  setupdisplay();
  setupSensors();
  setupTimer(100000); // Configura el temporizador con 0.1 segundo (100.000 microsegundos)
}

void loop() {
  loopSensing();
  loopSensors();
  loopControl();

  // Actualizar DISPLAY con valores sensados
  updateValues(battery1Temperature, battery1Voltage, battery1Current, battery2Temperature);

  // Aquí podrías agregar cualquier otra funcionalidad que necesites en el bucle principal.
}
