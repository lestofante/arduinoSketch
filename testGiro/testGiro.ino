#include "Struct.h"
#include "Gyro.h"
#include <Wire.h>

Gyro giro;

void setup(){
  Serial.begin(115200);
  Wire.begin();
  Serial.println("Starting giro");
  giro.init();
  Serial.println("Ok giro");
  delay(2000);
}

void loop(){
  Serial.println("loop");
  giro.update();
}
