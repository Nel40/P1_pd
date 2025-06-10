
El objetivo de esta práctica es producir el parpadeo periódico de un LED, controlado por un
microcontrolador ESP32. Además, se utilizará el puerto serie para depurar el programa y
monitorear el estado del LED.
Funcionalidad

1. Iniciar el pin del LED como salida: Se configura el pin para controlar el LED. En este
caso, se utiliza el pin 2 de la placa ESP32.
2. Bucle infinito: Se implementa un bucle donde el LED se enciende, espera 500
milisegundos, se apaga, y luego espera otros 500 milisegundos.
3. Enviar datos por puerto serie: El programa debe imprimir mensajes en el puerto serie
para indicar cuándo el LED está encendido ("ON") o apagado ("OFF").


Código Básico
```cpp
#define LED_BUILTIN 2 // Pin del LED
#define DELAY 500 // Tiempo de espera en milisegundos
void setup() {
pinMode(LED_BUILTIN, OUTPUT); // Iniciar pin del LED como
salida
Serial.begin(115200); // Iniciar comunicación serial
}
void loop() {
digitalWrite(LED_BUILTIN, HIGH); // Encender el LED
Serial.println("ON"); // Enviar mensaje "ON" por
puerto serie
delay(DELAY); // Espera de 500 milisegundos
C/C++
digitalWrite(LED_BUILTIN, LOW); // Apagar el LED
Serial.println("OFF"); // Enviar mensaje "OFF" por
puerto serie
delay(DELAY); // Espera de 500 milisegundos
}
```
Modificación: Acceso Directo a Registros
Para optimizar el programa, se puede modificar para que acceda directamente a los registros
de los puertos de entrada y salida de la ESP32, lo que elimina el uso de funciones como
digitalWrite().
Código con Acceso Directo a Registros
```cpp
#define LED_PIN 2 // Pin del LED
void setup() {
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG; // Puntero al
registro de salida
}
void loop() {
uint32_t *gpio_out = (uint32_t *)GPIO_OUT_REG; // Puntero al
registro de salida
*gpio_out |= (1 << LED_PIN); // Encender el LED
Serial.println("ON");
delay(1000); // Espera de 1000 milisegundos
*gpio_out &= ~(1 << LED_PIN); // Apagar el LED
Serial.println("OFF");
delay(1000); // Espera de 1000 milisegundos
}
```
Metodología
Para llevar a cabo esta práctica, se siguieron los siguientes pasos:
1. Configuración del entorno de desarrollo: Se utilizó PlatformIO como entorno de
desarrollo para programar la ESP32.
2. Implementación del programa para el parpadeo del LED: Se escribió el código
básico para hacer parpadear el LED con el uso de digitalWrite() y delay().
3. Pruebas con diferentes configuraciones:
○ Uso de funciones de Arduino (digitalWrite() y delay()).
○ Acceso directo a registros GPIO.
○ Con y sin envío de datos por puerto serie.
4. Medición de la frecuencia de parpadeo: Se utilizaron un osciloscopio y un generador
de señales para medir la frecuencia de parpadeo del LED en diferentes configuraciones.
5. Análisis de los resultados obtenidos: Se compararon las frecuencias de parpadeo en
los diferentes escenarios para evaluar el impacto de cada configuración.
Resultados y Análisis
A partir de las mediciones realizadas, se observa que:
1. Uso de digitalWrite(): Introduce retardos adicionales debido a la sobrecarga de la
función, lo que limita la frecuencia de parpadeo alcanzable.
2. Acceso directo a registros: Permite una mayor frecuencia de parpadeo al eliminar la
sobrecarga de las funciones de alto nivel. Esto optimiza el rendimiento al acceder
directamente a los registros de los puertos GPIO.
3. Envío de datos por puerto serie: La latencia de la transmisión de datos por el puerto
serie reduce la frecuencia máxima de parpadeo que puede alcanzarse.
