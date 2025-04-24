#include <Arduino.h>

struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
  unsigned long lastPressTime;  // Variable para almacenar el tiempo de la última pulsación
  unsigned long debounceDelay;  // Variable para definir el tiempo de rebote (debounce)
};

Button button1 = {18, 0, false, 0, 200};  // Tiempo de debounce a 200 ms

void IRAM_ATTR isr() {
  // Si el tiempo desde la última pulsación es mayor que el tiempo de rebote
  if (millis() - button1.lastPressTime > button1.debounceDelay) {
    button1.numberKeyPresses += 1;
    button1.pressed = true;
    button1.lastPressTime = millis();  // Actualiza el tiempo de la última pulsación
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(button1.PIN, INPUT_PULLUP);
  attachInterrupt(button1.PIN, isr, FALLING);  // Interrupción en flanco de bajada
}

void loop() {
  if (button1.pressed) {
    Serial.printf("Button 1 has been pressed %u times\n", button1.numberKeyPresses);
    button1.pressed = false;  // Resetear estado de presionado
  }

  // Desconectar la interrupción después de 1 minuto
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    detachInterrupt(button1.PIN);  // Desconectar la interrupción
    Serial.println("Interrupt Detached!");
  }
}
