
Informe de la Práctica 2: Interrupciones en
ESP32

Comprender el funcionamiento de las interrupciones en la ESP32 mediante eventos de
hardware (botón) y temporizadores, observando su comportamiento a través del monitor serie.
Se incorpora además la modificación de la frecuencia de parpadeo de un LED mediante
botones.
Funcionalidad
Interrupción por GPIO
● Se configura un botón en el pin GPIO 18 para generar una interrupción en flanco de
bajada.
● Se implementa un mecanismo de debounce para evitar lecturas erróneas.
● Se imprime en el monitor serie la cantidad de veces que el botón ha sido presionado.
Código relevante:
```cpp
attachInterrupt(button1.PIN, isr, FALLING); // Configura
interrupción en flanco de bajada
if (millis() - button1.lastPressTime > button1.debounceDelay) {
// Implementación de debounce
button1.numberKeyPresses++;
Serial.printf("Button 1 has been pressed %u times\n",
button1.numberKeyPresses);
}
```
Interrupción por Temporizador
● Se configura un temporizador para generar una interrupción cada segundo.
● Cada interrupción incrementa un contador e imprime el total en el monitor serie.
Código relevante:
C/C++
C/C++
```cpp
timer = timerBegin(0, 80, true); // Inicializa el temporizador
con preescalador 80
timerAttachInterrupt(timer, &onTimer, true); // Asocia la ISR al
temporizador
timerAlarmWrite(timer, 1000000, true); // Configura la
interrupción cada 1s
timerAlarmEnable(timer); // Habilita el temporizador
Serial.print("An interrupt has occurred. Total number: ");
Serial.println(totalInterruptCounter); // Muestra el conteo de
interrupciones
```
Modificación: Control de Frecuencia de Parpadeo
● Se añaden dos botones (GPIO 4 y GPIO 5) para aumentar o disminuir la frecuencia de
parpadeo del LED.
● Se utiliza un temporizador para alternar el estado del LED.
● Se implementa debounce en la lectura de los botones.
Código relevante:
```cpp
if (digitalRead(BUTTON_UP) == LOW && millis() - lastPressUp >
200) {
if (blinkFrequency < 10) blinkFrequency++; // Límite
superior
lastPressUp = millis();
}
if (digitalRead(BUTTON_DOWN) == LOW && millis() - lastPressDown >
200) {
if (blinkFrequency > 1) blinkFrequency--; // Límite inferior
lastPressDown = millis();
}
timerAlarmWrite(timer, 500000 / blinkFrequency, true); // Ajuste
dinámico del temporizador
```
C/C++
Ejercicio para subir nota
Descripción del Código Generado
El código generado por ChatGPT realiza las siguientes funciones:
● Configura el LED en el pin GPIO 2.
● Configura los botones en los pines GPIO 4 y GPIO 5 con resistencias pull-up internas.
● Usa un temporizador de hardware para controlar el parpadeo del LED.
● Implementa un mecanismo de debounce dentro de la ISR del temporizador para evitar
falsas lecturas en los botones.
● Ajusta dinámicamente la frecuencia de parpadeo del LED en función de la pulsación de
los botones.
Código relevante:
```cpp
#define LED_PIN 2
#define BUTTON_UP 4
#define BUTTON_DOWN 5
volatile int blinkFrequency = 2; // Frecuencia inicial en Hz
volatile bool ledState = false;
hw_timer_t *timer = NULL;
void IRAM_ATTR onTimer() {
static unsigned long lastPressUp = 0;
static unsigned long lastPressDown = 0;
// Cambia el estado del LED
ledState = !ledState;
digitalWrite(LED_PIN, ledState);
// Leer los botones con debounce
if (digitalRead(BUTTON_UP) == LOW && millis() - lastPressUp >
200) {
if (blinkFrequency < 10) blinkFrequency++; // Límite
superior
lastPressUp = millis();
}
if (digitalRead(BUTTON_DOWN) == LOW && millis() -
lastPressDown > 200) {
if (blinkFrequency > 1) blinkFrequency--; // Límite
inferior
lastPressDown = millis();
}
// Actualizar el periodo del timer
timerAlarmWrite(timer, 500000 / blinkFrequency, true);
}
void setup() {
pinMode(LED_PIN, OUTPUT);
pinMode(BUTTON_UP, INPUT_PULLUP);
pinMode(BUTTON_DOWN, INPUT_PULLUP);
// Configurar el timer (0) con una interrupción cada 500ms
(ajustable)
timer = timerBegin(0, 80, true);
timerAttachInterrupt(timer, &onTimer, true);
timerAlarmWrite(timer, 500000 / blinkFrequency, true);
timerAlarmEnable(timer);
}
```
Resultados y Análisis
● Interrupción por GPIO: El botón genera interrupciones correctas, y el debounce evitó
lecturas erróneas.
● Interrupción por Temporizador: Generación precisa de interrupciones cada segundo,
con el contador reflejando los eventos en el monitor serie.
● Control de Frecuencia: La modificación permite cambiar la velocidad de parpadeo del
LED de forma dinámica.
