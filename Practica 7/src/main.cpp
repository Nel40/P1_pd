#include <Arduino.h>
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourcePROGMEM.h"
#include "sampleaac.h"


AudioFileSourcePROGMEM *in;
AudioGeneratorAAC *aac;
AudioOutputI2S *out;


void setup() {
   Serial.begin(115200);
   in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
   aac = new AudioGeneratorAAC();
   out = new AudioOutputI2S();
   out->SetGain(0.125);
   out->SetPinout(7, 6, 8);  // Pines actualizados para ESP32-S3
   aac->begin(in, out);
}


void loop() {
   if (aac->isRunning()) {
       aac->loop();
   } else {
       aac->stop();
       Serial.println("Sound Generator");
       delay(1000);
   }
}
