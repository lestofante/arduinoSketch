#include "ITG3200.h"

ITG3200 giro;

#define ITG_ADDR 0xD0

void setup(){
  Serial.begin(19200);
  delay(3000);
  giro.begin(ITG_ADDR);
}

void loop(){
}
