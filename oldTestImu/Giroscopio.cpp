#import "Giroscopio.h"



static const float giroToRadiantAtSecond = (((5.0/1024.0)*1000.0)/0.67)/57.29578;
static const float giroToDegreeAtSecond = (((5.0/1024.0)*1000.0)/0.67);

float Giroscopio::setPin(int pin){
  gyro=0;
  gyroPin=pin;
  sommaMedia=0;
  int i, r;
  for (i=0; i < midPrecision;i++){
    sommaMedia += analogRead(gyroPin);
  }
  Serial.print("Media gyro:");
  Serial.println(sommaMedia/midPrecision);
  return sommaMedia/midPrecision;
}

float Giroscopio::leggi(){
  return gyro;
}

void Giroscopio::update(){
  int letto=analogRead(gyroPin);
  float actualMid = sommaMedia/midPrecision;
  gyro=letto-actualMid;
  sommaMedia-=actualMid;
  sommaMedia+=letto;
}
