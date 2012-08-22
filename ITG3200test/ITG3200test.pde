#include "ITG3200.h"
#include "twi.h"

void setup(){
  Serial.begin(19200);
  //Init gyro for 100 kHz and full-scale operation
  gyro.begin();
  twiReset();
  Serial.println("ok");
  delay(100);
}
