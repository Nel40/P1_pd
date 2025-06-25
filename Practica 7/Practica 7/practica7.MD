# PRACTICA 7 :  Buses de comunicación III (I2S)  

El objetivo de la practica actual es describir el funcionamiento del bus I2S y realizar una practica para comprender su  funcionamiento 

## Ejercicio Practico 1  Reproducción desde memoria interna

### 1. Descibir la salida por el puerto serie 

El código utilizado es el siguiente:
```c
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"

AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;

void setup()
{
    Serial.begin(115200);
    in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
    aac = new AudioGeneratorAAC();
    out = new AudioOutputI2S();
    out -> SetGain(0.125);
    out -> SetPinout(26,25,22);
    aac->begin(in, out);
}
void loop()
{
    if (aac->isRunning()) 
    {
        aac->loop();
    } 
    else 
    {
        aac -> stop();
        Serial.printf("Sound Generator\n");
        delay(1000);
    }
}
```

Este código es un ejemplo de cómo reproducir un archivo de audio codificado en AAC (Advanced Audio Coding) utilizando un microprocesador y un ADC (Analog to Digital Converter) MAX98357 I2S para decodificar la señal digital en una señal analógica.
Por la salida del puerto serie tendremos los datos analógicos necesarios para la reproducción del audio en los altavoces.
En nuestro caso, el audio utilizado es el llamado `sampleaac`, que una vez reproducido podemos escuchar a Homer de Los Simpsons hablando de la termodinámica ("¡En esta casa obedecemos las leyes de la TERMODINÁMICA!").

### 2. Explicar el funcionamiento 
1. Librerías:
   - `AudioGeneratorAAC.h`: Biblioteca para generar audios a partir de archivos AAC.
   - `AudioOutputI2S.h`: Biblioteca para la salida de audio a través del protocolo I2S.
   - `AudioFileSourcePROGMEM.h`: Biblioteca para acceder a archivos de audio almacenados en la memoria del programa (PROGMEM).
2. Inicialización de objetos y variables:
   - `AudioFileSourcePROGMEM *in;`: Puntero al objeto AudioFileSourcePROGMEM que representa la fuente del archivo de audio almacenado en PROGMEM.
   - `AudioGeneratorAAC *aac;`: Puntero al objeto AudioGeneratorAAC que se encarga de decodificar y reproducir los archivos AAC.
   - `AudioOutputI2S *out;`: Puntero al objeto AudioOutputI2S que maneja la salida del audio a través del protocolo I2S.
3. Función `setup()`:
   - Inicialización de la comunicación serial a 115200 baudios.
   - `in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));`: Crea una instancia de AudioFileSourcePROGMEM para acceder al archivo de audio almacenado llamado `sampleaac`, el cual es un conjunto de bytes codificados.
   - `aac = new AudioGeneratorAAC();`: Crea una instancia de AudioGeneratorAAC para decodificar y reproducir archivos AAC.
   - `out = new AudioOutputI2S();`: Crea una instancia de AudioOutputI2S para configurar la salida de audio.
   - `out -> SetGain(0.125);`: Configura el nivel de ganancia de la salida.
   - `out -> SetPinout(26, 25, 22);`: Configura los pines utilizados para la salida del audio en I2S.
4. Función `loop()`:
   - Verifica si el generador de audio AAC está en ejecución utilizando `aac -> isRunning()`.
   - En el caso de que se esté ejecutando, se llama a `aac -> loop()` para procesar y reproducir el audio.
   - En caso contrario, el generador no está en ejecución y, por lo tanto, se detiene con `aac->stop()`.
   - Se imprime un mensaje y se espera 1 segundo para volver a repetir todo el proceso.
