#include "CONTROLHandler.h"
#include "CONCIENCIAHandler.h"
#include "SENSORHandler.h"

// Definición de pines y variables globales
const int contactorPin1 = 02; // El pin conectado al relé-contactor para cortar la energía principal
const int contactorPin2 = 15; // El pin conectado al relé-contactor para cortar la energía de paneles solares
const int buttonPin1 = 14; // Pin para el botón que apaga el relé 1
const int buttonPin2 = 13; // Pin para el botón que apaga el relé 2
const int buttonPin3 = 12; // Pin para el botón que desactiva el sensor de conciencia
bool contactorState1 = LOW; // Estado del relé-contactor 1 (HIGH o LOW)
bool contactorState2 = LOW; // Estado del relé-contactor 2 (HIGH o LOW)

void setupControl() {
  pinMode(contactorPin1, OUTPUT); // Configura el pin del relé-contactor 1 como salida
  pinMode(contactorPin2, OUTPUT); // Configura el pin del relé-contactor 2 como salida
  digitalWrite(contactorPin1, LOW); // Estado inicial relé-contactor 1
  digitalWrite(contactorPin2, LOW); // Estado inicial relé-contactor 2

  pinMode(buttonPin1, INPUT_PULLDOWN); // Configura el pin del botón 1 como entrada
  pinMode(buttonPin2, INPUT_PULLDOWN); // Configura el pin del botón 2 como entrada
  pinMode(buttonPin3, INPUT_PULLDOWN); // Configura el pin del botón 3 como entrada
}

void loopControl() {
  // Verificar el estado de los botones
  // Verificar el estado de los botones
  if (digitalRead(buttonPin1) == HIGH) {
    contactorState1 = LOW;
    digitalWrite(contactorPin1, contactorState1);
    Serial.println("Contactor 1 OFF by button");
  }

  if (digitalRead(buttonPin2) == HIGH) {
    contactorState2 = LOW;
    digitalWrite(contactorPin2, contactorState2);
    Serial.println("Contactor 2 OFF by button");
  }

  if (digitalRead(buttonPin3) == HIGH) {
    isSensorEnabled = !isSensorEnabled; // Alternar el estado del sensor de conciencia
    Serial.print("Sensor de conciencia ");
    Serial.println(isSensorEnabled ? "enabled" : "disabled");
  }

  if (isUnconscious && isSensorEnabled) {
    if (!contactorState1) {
      contactorState1 = LOW;
      digitalWrite(contactorPin1, contactorState1);
      Serial.println("Contactor 1 OFF due to unconsciousness");
    }
    if (!contactorState2) {
      contactorState2 = LOW;
      digitalWrite(contactorPin2, contactorState2);
      Serial.println("Contactor 2 OFF due to unconsciousness");
    }
  } else {
    if (contactorState1) {
      contactorState1 = LOW;
      digitalWrite(contactorPin1, LOW);
      Serial.println("Contactor 1 OFF");
    }
    if (contactorState2) {
      contactorState2 = LOW;
      digitalWrite(contactorPin2, LOW);
      Serial.println("Contactor 2 OFF");
    }
  }

  if (battery1Temperature >= 53) {
    contactorState2 = LOW;
    digitalWrite(contactorPin2, contactorState2);
    Serial.println("Contactor 2 OFF due to temperature");
  }
}

bool debounce(int pin) {
  static unsigned long lastDebounceTime = 0;
  static bool lastButtonState = HIGH;
  bool reading = digitalRead(pin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > 50) {
    if (reading == LOW) {
      lastButtonState = reading;
      return true;
    }
  }

  lastButtonState = reading;
  return false;
}
