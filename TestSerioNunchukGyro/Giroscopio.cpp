#import "Giroscopio.h"

static const float giroToRadiantAtSecond = (((5.0/1024.0)*1000)/0.67)/57.29578;

float Giroscopio::setPin(int pin){
  gyro=0;
  gyroPin=pin;
  mid = analogRead(gyroPin);
  int i;
  for (i=0; i < 1000;i++){
    mid = ( mid+analogRead(gyroPin) )/2;
  }
  return mid;
}

float Giroscopio::leggi(){
  return gyro;
}

void Giroscopio::update(){
  gyro=analogRead(gyroPin);
  if (mid-1<gyro && gyro<mid+1){ //if mid-1<gyro<mid+1 then gyro=mid
    gyro=mid;
  }
  gyro = (gyro-mid)*giroToRadiantAtSecond;
}
