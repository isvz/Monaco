#include "TIMERHandler.h"

// Inicialización de la variable del temporizador y la bandera de disparo del temporizador
hw_timer_t* my_timer = nullptr;
bool timer_fired = false;

// Definición de la función del ISR
void IRAM_ATTR onTimer() {
    timer_fired = true;     // No usar Serial.print() en una ISR o cualquier función intensiva en recursos
}

// Configuración del temporizador
void setupTimer(int frequency) {
    my_timer = timerBegin(1, 800, true); // Suponiendo un reloj de 80MHz
    timerAttachInterrupt(my_timer, &onTimer, true);
    timerAlarmWrite(my_timer, frequency, true);
    timerAlarmEnable(my_timer);
}
