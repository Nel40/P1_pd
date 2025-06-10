Informe de la Práctica 6: Buses de Comunicación II (SPI)

Comprender el funcionamiento del bus de comunicación SPI (Serial Peripheral Interface) en
aplicaciones con ESP32. Se realizarán ejercicios prácticos de lectura de memoria SD y lectura
de etiquetas RFID utilizando el protocolo SPI.
Introducción Teórica
El bus SPI es un protocolo síncrono, full-duplex, maestro-esclavo, de alta velocidad (hasta 80
MHz en ESP32) desarrollado originalmente por Motorola. Requiere:
● MOSI (Master Out Slave In)
● MISO (Master In Slave Out)
● SCK (Serial Clock)
● SS (Slave Select) por cada esclavo
Ventajas del SPI
● Alta velocidad de transmisión.
● Arquitectura simple.
● Posibilidad de enviar datos de cualquier tamaño.
Desventajas
● Necesita una línea SS por cada dispositivo esclavo.
● No hay confirmación de recepción de datos.
● Limitado a cortas distancias.

SPI en ESP32/ESP32-S3
● El ESP32 tiene 2 buses SPI disponibles para el programador.
● El ESP32-S3 dispone de 2 buses SPI libres.
Conexiones por defecto ESP32-S3:
● SPI2: MOSI(GPIO 35), MISO(GPIO 37), CLK(GPIO 36), CS(GPIO 39)
● SPI3: MOSI(GPIO 11), MISO(GPIO 13), CLK(GPIO 12), CS(GPIO 10)
Ejercicio Práctico 1: Lectura/Escritura de Memoria SD
Objetivo
Leer el contenido de un archivo almacenado en una tarjeta SD conectada vía SPI.
Código 

```cpp
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

// Definir pines para SPI
#define SD_MOSI 11
#define SD_MISO 12
#define SD_SCK 13
#define SD_CS 4

void listFiles(File dir, int numTabs);

void setup() {
    Serial.begin(115200);
    
    // Configurar bus SPI
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    
    if (!SD.begin(SD_CS)) {
        Serial.println("Error al inicializar la tarjeta SD");
        return;
    }
    Serial.println("Tarjeta SD inicializada correctamente");
    
    File root = SD.open("/");
    Serial.println("Contenido de la tarjeta SD:");
    listFiles(root, 0);
}

void loop() {
    // No es necesario código en loop para solo listar archivos
}

void listFiles(File dir, int numTabs) {
    while (true) {
        File entry = dir.openNextFile();
        if (!entry) {
            break;
        }
        for (int i = 0; i < numTabs; i++) {
            Serial.print("\t");
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            listFiles(entry, numTabs + 1);
        } else {
            Serial.print(" \tTamaño: ");
            Serial.println(entry.size());
        }
        entry.close();
    }
}
```
Funcionamiento
● Se inicializa la comunicación SPI con la tarjeta SD.
● Se abre un archivo llamado archivo.txt.
● Si el archivo existe, se muestra su contenido por el puerto serie.
● Si no, se notifica el error.
Salida en el monitor serie
Iniciando SD ...
Inicializacion exitosa
archivo.txt:
(Ejemplo del contenido del archivo)
C/C++
Unset
Ejercicio Práctico 2: Lectura de Etiqueta RFID
Objetivo
Leer el UID de tarjetas RFID utilizando el módulo RC522 conectado vía SPI.
Código Destacado
```cpp
#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9 //Pin 9 para el reset del RC522
#define SS_PIN 10 //Pin 10 para el SS (SDA) del RC522
MFRC522 mfrc522(SS_PIN, RST_PIN); //Creamos el objeto para el RC522
void setup() {
  Serial.begin(115200); //Iniciamos la comunicación serial
  SPI.begin(36, 37, 35); //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Lectura del UID");
}
void loop() {
  // Revisamos si hay nuevas tarjetas presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
  { 
    //Seleccionamos una tarjeta
    if ( mfrc522.PICC_ReadCardSerial()) 
    {
      // Enviamos serialemente su UID
      Serial.print("Card UID:");
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0"
        : " ");
        Serial.print(mfrc522.uid.uidByte[i], HEX); 
      } 
      Serial.println();
      // Terminamos la lectura de la tarjeta actual
      mfrc522.PICC_HaltA(); 
    } 
  } 
}
```

Funcionamiento
● Se inicia la comunicación SPI y el lector RFID.
● Se verifica la presencia de una tarjeta.
● Se lee el UID de la tarjeta y se imprime por el puerto serie.
Salida en el monitor serie
Lectura del UID
Card UID: XX XX XX XX

Ejercicio Práctico 3: Trabajo en Casa
● Utilizar otro dispositivo SPI disponible.
● Documentar con fotos del montaje, salidas de depuración, código y explicación.

Ejercicio de Subida de Nota
Parte 1: Registro de Lecturas RFID en la SD
● Usar un único bus SPI para controlar el lector RFID y la tarjeta SD.
● Escribir un fichero .log donde se registre la hora y el UID de cada lectura.
Parte 2: Web Server para Ver Lecturas
● Crear una página web alojada en la ESP32.
● Mostrar las últimas lecturas RFID.
Conclusiones
● El bus SPI es muy eficiente para aplicaciones de alta velocidad y de corta distancia.
● El ESP32 facilita la utilización de múltiples dispositivos SPI.
● Es necesario gestionar correctamente la selección de dispositivos mediante las líneas
SS.
● La combinación de módulos como SD y RFID permite crear sistemas de registro y
control avanzados.
