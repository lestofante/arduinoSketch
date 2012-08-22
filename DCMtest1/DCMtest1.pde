#include "IMU.h"

IMU orizzonte;

unsigned long time;
void setup(){
  Serial.begin(19200);
  time = micros();
}
unsigned long time2=0;
unsigned long time3=0;
int loopNumber=0;
void loop(){
  loopNumber++;
  time3=micros();
  if (time2==0)
    time2=time3-time;
  else
    time2=(time2+ (time3-time) )/2;
  time = time3;
  if (loopNumber > 100){
    Serial.println(time2);
    loopNumber=0;
  }
  
  float gyroX;
  float gyroY;
  float gyroZ;
  
  float accX;
  float accY;
  float accZ;
  
  //normalizzazione valori
  
  orizzonte.IMUupdate(gyroX, gyroY, gyroZ, accX, accY, accZ);
}
