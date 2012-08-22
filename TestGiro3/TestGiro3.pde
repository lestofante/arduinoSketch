#include "Giroscopio.h"

#define gyroXpin 0
#define gyroYpin 1
#define gyroZpin 2

Giroscopio gyro[3];

unsigned long time1;
unsigned long time;
void setup(){
  Serial.begin(19200);
  //setting gyro pin, automagigally they calibration
  gyro[0].setPin(gyroXpin);
  //  gyro[1].setPin(gyroYpin);
  //  gyro[2].setPin(gyroZpin);
  time = time1 = micros();
}

unsigned long time2;
long loopNumber=0;

float angle=0;

void loop(){
  time2=micros();
  gyro[0].update(time2-time);
  //  gyro[1].update();
  //  gyro[2].update();
  time1=time2;
/*
  angle*=gyro[0].leggi();
  loopNumber++;

  while (angle < 0)
    angle += 360;
  while (angle >= 360)
    angle -= 360;

  if (time2-time>1000000L){
    //Serial.print("X Y Z:");
    Serial.print( gyro[0].leggi() );

     Serial.print(" ");
     Serial.print( gyro[1].leggi() );
     Serial.print(" ");
     Serial.print( gyro[2].leggi() );

    Serial.println();
    Serial.print( "loop number" );
    Serial.print( loopNumber );
    Serial.print( " angle" );
    Serial.print( angle );
    Serial.println();
    loopNumber=0;
    time=micros();
  }
*/
}

