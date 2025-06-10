Informe de la Práctica 4: Sistemas Operativos en Tiempo Real

El objetivo de esta práctica es comprender el funcionamiento de un sistema operativo en tiempo real (RTOS)
aplicando FreeRTOS en la ESP32. Se pretende familiarizarse con la creación, gestión y ejecución de tareas
simultáneas, así como con el uso de prioridades, anclaje a núcleos y sincronización entre tareas.
Introducción Teórica
La ESP32 cuenta con un procesador de doble núcleo y es compatible con FreeRTOS, lo que permite dividir el uso
del CPU entre varias tareas concurrentes. Con FreeRTOS es posible:
● Ejecutar tareas en paralelo (multitarea).
● Asignar prioridades a tareas.
● Controlar en qué núcleo se ejecutan las tareas.
● Detener tareas.
● Sincronizar tareas mediante semáforos o mutex.
Fundamentos de FreeRTOS en ESP32
Creación de Tareas
Para crear una tarea en FreeRTOS se utiliza xTaskCreate o xTaskCreatePinnedToCore, y se proporciona:
● La función a ejecutar.
● El nombre de la tarea.
● El tamaño de la pila.
● El parámetro de entrada.
● La prioridad.
● Un manejador para la tarea (opcional).
●

Ejecución en Núcleos Específicos
Se puede especificar en qué núcleo ejecutar una tarea usando xTaskCreatePinnedToCore, agregando el último
parámetro con valor 0 o 1:
```cpp

xTaskCreatePinnedToCore(
 myTask,
 "My Task",
 1000,
 NULL,
 1,
 NULL,
 1
);
```
Finalización de una tarea
Una tarea se puede finalizar con vTaskDelete(NULL) desde dentro, o utilizando un TaskHandle_t desde otra
tarea o función.
Prioridades
FreeRTOS permite establecer prioridades de ejecución. Una prioridad más alta (mayor número) significa que la tarea
tiene preferencia cuando varias tareas compiten por el uso de la CPU.
Ejercicio Práctico 1: Tarea concurrente
Código Implementado
Se crea una tarea adicional anotherTask desde setup() mientras loop() imprime un mensaje cada segundo:
```cpp
void setup() {
 Serial.begin(115200);
 xTaskCreate(
 anotherTask,
 "another Task",
 10000,
 NULL,
 1,
 NULL
 );
Unset
}
void loop() {
 Serial.println("this is ESP32 Task");
 delay(1000);
}
void anotherTask(void * parameter) {
 for (;;) {
 Serial.println("this is another Task");
 delay(1000);
 }
 vTaskDelete(NULL);
}
```
Funcionamiento
● La función setup() inicia el puerto serie y lanza una tarea adicional.
● loop() imprime continuamente un mensaje indicando que está ejecutando la tarea por defecto de Arduino.
● La tarea anotherTask se ejecuta en paralelo, imprimiendo también un mensaje.
Salida en el monitor serie
this is ESP32 Task
this is another Task
this is ESP32 Task
this is another Task
...
Ambas tareas se ejecutan de forma concurrente, compartiendo el tiempo del procesador.
Conclusiones
● FreeRTOS facilita la ejecución concurrente de tareas, mejorando la eficiencia y capacidad de respuesta del
sistema.
● Las tareas pueden ejecutarse en diferentes núcleos y con prioridades distintas.
● La división del tiempo de CPU entre tareas es controlada por el planificador de FreeRTOS.
● Los semáforos permiten sincronizar tareas de forma ordenada.
Esta práctica permite comprender los conceptos fundamentales de los sistemas operativos en tiempo real aplicados
a sistemas embebidos como el ESP32.
