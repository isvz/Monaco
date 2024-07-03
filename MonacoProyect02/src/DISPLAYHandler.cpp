#include "DISPLAYHandler.h"

// Configuración de la dirección del módulo I2C y tamaño de la pantalla LCD
int lcdAddr = 0x27;
int lcdColumns = 20;
int lcdRows = 4;

// Inicialización de la pantalla LCD
LiquidCrystal_PCF8574 lcd(lcdAddr);

void setupdisplay() {
  Wire.begin(); // Inicializar la comunicación I2C
  lcd.begin(lcdColumns, lcdRows); // Conectar con la pantalla LCD
  lcd.setBacklight(255); // Encender la retroiluminación

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Temp Bat 1:");
  lcd.setCursor(0, 1); lcd.print("VB1/IB1:");
  lcd.setCursor(0, 2); lcd.print("Temp Bat 2:");
  lcd.setCursor(0, 3); lcd.print("%B1/%B2:");
}


void updateValues(float temp_bateria1, float voltaje_B1, float corriente_B1, float temp_bateria2) {
  lcd.setCursor(12, 0);
  lcd.print(String(temp_bateria1, 1) + "C");
  lcd.setCursor(10, 1);
  lcd.print(String(voltaje_B1, 1) + "V " + String(corriente_B1, 1) + "A");
  lcd.setCursor(12, 2);
  lcd.print(String(temp_bateria2, 1) + "C");
  lcd.setCursor(9, 3);
  lcd.print(String(70, 1) + "% " + String(70, 1) + "%");
}
