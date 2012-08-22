#include "IMU.h"

#include "Giroscopio.h"
#include "NunchukFarlocco.h"

#define gyroXpin 1
#define gyroYpin 2
#define gyroZpin 3

Giroscopio gyro[3];
NunchukFarlocco nun;
float ris[6];

IMU::IMU(){
  
}

void IMU::start(){
  gyro[0].setPin(gyroXpin);
  gyro[1].setPin(gyroYpin);
  gyro[2].setPin(gyroZpin);
  nun.nunchuck_init(); // send the initilization handshake
}

unsigned long lastUpdateTime = 0;
void IMU::update(){
  //unsigned long time = micros();
  lastUpdateTime++;
  gyro[0].update();
  gyro[1].update();
  gyro[2].update();
  
  /*
  time = micros()-time;
  Serial.print("gyro time:");
  Serial.println(time);
  */
//  if ( lastUpdateTime+0 < millis() ){
    //lastUpdateTime=millis();
    //time = micros();
  if (3<=lastUpdateTime){
    nun.update();
    lastUpdateTime=0;
  }
    /*
    time = micros()-time;
    Serial.print("acc time:");
    Serial.println(time);  
    */
  //}
}

float* IMU::leggi(){
  int i;
  float* nunTemp = nun.leggi();
  for (i=0; i < 3; i++){
    ris[i]=gyro[i].leggi();//put gyro data in ris[0],[1],[2]
    
    ris[i+3]=nunTemp[i];//put nun data in ris[3],[4],[5]
  }
  return ris;
}
