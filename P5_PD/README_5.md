
Informe de la Práctica 5: Buses de Comunicación I (I2C)

El objetivo de esta práctica es comprender el funcionamiento de los buses de comunicación entre periféricos, en
particular el bus I2C, mediante la programación en la ESP32 y la interacción con diferentes dispositivos conectados
a través de dicho bus.

El bus I2C (Inter-Integrated Circuit) es un protocolo de comunicación síncrono, serie y half-duplex que permite
conectar múltiples dispositivos mediante dos líneas:
● SDA (datos)
● SCL (reloj)
Cada dispositivo en el bus tiene una dirección única. El maestro (ESP32 en este caso) inicia la comunicación y
controla el reloj. Es posible conectar hasta 112 dispositivos con direcciones distintas.
El protocolo permite leer o escribir datos con verificación de llegada, pero sin comprobación del contenido.
Ejercicio 1: Escáner I2C

Codigo impementado
```cpp
#include <Arduino.h>
#include <Wire.h>
void setup()
{
Wire.begin();
Serial.begin(115200);
while (!Serial);
Serial.println("\nI2C Scanner");
}
void loop()
{
byte error, address;
int nDevices;
Serial.println("Scanning...");
nDevices = 0;
for(address = 1; address < 127; address++ )
{
Wire.beginTransmission(address);
error = Wire.endTransmission();
if (error == 0)
{
Serial.print("I2C device found at address 0x");
if (address<16)
Serial.print("0");
Serial.print(address,HEX);
Serial.println(" !");
nDevices++;
}
else if (error==4)
{
Serial.print("Unknown error at address 0x");
if (address<16)
Serial.print("0");
Serial.println(address,HEX);
}
}
if (nDevices == 0)
Serial.println("No I2C devices found\n");
else
Serial.println("done\n");
delay(5000);
}
```
Objetivo
Detectar los dispositivos conectados al bus I2C utilizando la ESP32.
Salida
I2C Scanner
Scanning...
I2C device found at address 0x3C
I2C device found at address 0x57

Explicación
Este código recorre las direcciones del bus I2C desde la 1 a la 126. Si un dispositivo responde, se imprime su
dirección. Esto permite identificar qué dispositivos están correctamente conectados.
Ejercicio 2: Uso de un Dispositivo I2C - Pantalla OLED
SSD1306

Objetivo
Visualizar gráficos y texto en una pantalla OLED conectada por I2C.
Librerías
● Adafruit_SSD1306
● Adafruit_GFX
Explicación
Se inicializa la pantalla con su dirección I2C, se limpia el buffer y se imprime texto. Las funciones de la librería
permiten también dibujar formas, imágenes, animaciones y texto estilizado.
Ejercicio Adicional: Sensor de Frecuencia Cardíaca
MAX30102

Objetivo
Leer el nivel de oxígeno en sangre y frecuencia cardíaca utilizando el sensor MAX30102 mediante I2C.
Librerías
● MAX30105
● spo2_algorithm.h

Funcionalidad
● Lee valores infrarrojos y rojos.
● Calcula ritmo cardiaco y saturación de oxígeno.
● Muestra los resultados por el puerto serie.

Salida
red=42850, ir=56834, HR=72, HRvalid=1, SPO2=98, SPO2Valid=1
Parte Avanzada (Subida de Nota)
1. Mostrar frecuencia cardiaca y oxígeno en la pantalla OLED.
2. Crear una página web para visualizar estos datos remotamente.
Conclusiones
● El bus I2C permite comunicación simple y eficiente entre múltiples dispositivos.
● Usar un escáner permite detectar direcciones activas y facilita la depuración.
● Las pantallas OLED ofrecen una forma visual de mostrar información sin necesidad de un monitor.
● El sensor MAX30102 combinado con un display forma una estación biomédica compacta.
