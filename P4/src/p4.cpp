#include <Arduino.h>

void anotherTask( void * parameter );
void setup()
{
Serial.begin(112500);
xTaskCreate(anotherTask, "another Task", 10000, NULL, 1, NULL); 
}
void loop()
{
Serial.println("this is ESP32 Task");
delay(1000);
}
void anotherTask( void * parameter )
{
for(;;)
{
Serial.println("this is another Task");
delay(1000);
}
vTaskDelete( NULL );
}
