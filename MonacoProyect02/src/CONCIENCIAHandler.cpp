#include "CONCIENCIAHandler.h"
#include "TIMERHandler.h"

// Definición de pines y variables globales
const int touchPin = 32; // El pin que se usará para el sensor táctil
const int bocinaPin = 4; // El pin conectado al relé para la bocina
const int threshold = 20; // Umbral de sensibilidad del sensor táctil
unsigned long timeThresholdMet = 0; // Almacena el momento en que se cumple el umbral
unsigned long lastToggleTime = 0; // La última vez que el estado de la bocina cambió
int stage = 0; // Etapa actual de la alarma
bool bocinaState = LOW; // Estado actual de la bocina (HIGH o LOW)
const int numReadings = 10; // Número de lecturas para la media móvil
int readings[numReadings]; // Array para almacenar las lecturas
int readIndex = 0; // Índice actual para el próximo dato
int total = 0; // Suma total de las lecturas
int average = 0; // Promedio de las lecturas

bool isUnconscious = false;
bool isSensorEnabled = true; // Variable para habilitar o deshabilitar el sensor de conciencia

void setupSensing() {
  pinMode(bocinaPin, OUTPUT); // Configura el pin del relé como salida
  digitalWrite(bocinaPin, LOW); // Asegura que la bocina esté desactivada inicialmente

  // Inicializa todas las lecturas a 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }

  // Configura el temporizador
  setupTimer(100000); // Configura el temporizador con 0.1 segundo (100.000 microsegundos)
}

void loopSensing() {
  if (!isSensorEnabled) return; // Si el sensor de conciencia está deshabilitado, salir

  if (timer_fired) {
    timer_fired = false; // Restablecer la bandera

    total = total - readings[readIndex];
    readings[readIndex] = touchRead(touchPin);
    total = total + readings[readIndex];
    readIndex = (readIndex + 1) % numReadings;
    average = total / numReadings; // Media móvil

    if (average > threshold) {
      if (timeThresholdMet == 0) { // Comienza a contar si es la primera vez que se detecta el umbral
        timeThresholdMet = millis();
        lastToggleTime = millis();
        stage = 1; // Comienza en la primera etapa
      }

      unsigned long elapsedTime = millis() - timeThresholdMet;

      // Ajusta la etapa de la alarma según el tiempo transcurrido
      if (elapsedTime >= 10000 && stage < 3) {
        stage = 3; // Etapa de alta frecuencia
      } else if (elapsedTime >= 5000 && stage < 2) {
        stage = 2; // Etapa de media frecuencia
      }

      // Parpadeo de la bocina según la etapa
      if (millis() - lastToggleTime >= (stage == 3 ? 250 : (stage == 2 ? 500 : 1000))) {
        bocinaState = !bocinaState;
        digitalWrite(bocinaPin, bocinaState);
        lastToggleTime = millis();
        Serial.println(bocinaState ? "HIGH" : "LOW");
      }

      // Si alcanza la etapa 3, marcar como inconsciente
      if (stage == 3) {
        isUnconscious = true;
      }
    } else {
      if (bocinaState || timeThresholdMet != 0) { // Si la bocina estaba activa o se había cumplido el umbral
        bocinaState = LOW;
        digitalWrite(bocinaPin, LOW);
        timeThresholdMet = 0; // Restablece el temporizador de umbral
        stage = 0; // Restablece la etapa de alarma
        Serial.println("LOW");
      }
      isUnconscious = false;
    }
  }
}
