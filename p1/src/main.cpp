#include <Arduino.h>

#define LED_BUILTIN 2  // Pin del LED (cambiar según la placa)
#define DELAY 1000  // Tiempo de espera en milisegundos

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);  // Configurar el pin como salida
    Serial.begin(115200);  // Iniciar comunicación serie a 115200 baudios
    delay(1000);  // Pequeña pausa para que el monitor serie se estabilice
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);  // Encender LED
    Serial.println("ON");  // Enviar mensaje al puerto serie
    delay(DELAY);  // Esperar 1 segundo

    digitalWrite(LED_BUILTIN, LOW);  // Apagar LED
    Serial.println("OFF");  // Enviar mensaje al puerto serie
    delay(DELAY);  // Esperar 1 segundo
}
