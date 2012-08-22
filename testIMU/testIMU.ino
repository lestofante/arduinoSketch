#include <Wire.h>
#include "SensorDataStruct.h"
#include "ITG3200.h"
#include "ADXL345.h"
#include "HMC5883L.h"

#include "DCM.h"
#include "MahonyAHRS.h"
#include "MemoryFree.h"

struct data dataSensor;
struct data dataCopy;

Sensor *sensori[3];

DCM stabilizzatore;
MahonyAHRS stabilizzatore2;

uint8_t * heapptr, * stackptr;


unsigned long loopTime, tmpLoopTime;
void setup(){
  Serial.begin(19200);
  
  Serial.print("ok Seriale, check_mem1: ");
  Serial.println(freeMemory());
  
  loopTime = micros();
  Wire.begin();
  
  Serial.print("ok Wire, check_mem2: ");
  Serial.println(freeMemory());
  
  loopTime = micros();
  delay(1500);
  sensori[0]=new ITG3200();
  dataSensor.giroX=-1.0;
  dataSensor.giroY=-1.0;
  dataSensor.giroZ=-1.0;
  
  Serial.print("ok ITG, check_mem3: ");
  Serial.println(freeMemory());
  
  loopTime = micros();
  sensori[1]=new ADXL345();
  dataSensor.accX=0.0;
  dataSensor.accY=0.0;
  dataSensor.accZ=0.0;

  Serial.print("ok ADXL, check_mem4: ");
  Serial.println(freeMemory());
  
  loopTime = micros();
  sensori[2]=new HMC5883L();
  dataSensor.magneX=0.0;
  dataSensor.magneY=0.0;
  dataSensor.magneZ=0.0;

  Serial.print("ok HCM, check_mem5: ");
  Serial.println(freeMemory());

  loopTime = micros();
  for (int i =0; i < 3; i++){
    (*sensori[i]).setup(&dataSensor);
    (*sensori[i]).calibrate();
    (*sensori[i]).debug();
  }
  delay(500);
  
  Serial.print("ok calibrazione, check_mem6: ");
  Serial.println(freeMemory());
  
  stabilizzatore2.begin(2.0, 0.005);
  
  loopTime = micros();
}

long upGiro = 0;
long upAcc = 0;
long upMagne = 0;

long realUpGiro = 0;
long realUpAcc = 0;
long realUpMagne = 0;

char command = 'B';
unsigned long lastMillis = millis(), tempMillis, lastMillisDebugIMU;

void loop(){
  switch (Serial.read()){
    case 'A':
      command = 'A';
      Serial.println("Only ac");
      break;
    case 'B':
      command = 'B';
      Serial.println("ALL");
      break;
    case 'G':
      command = 'G';
      Serial.println("Only ac");
      break;
    case 'M':
      command = 'M';
      Serial.println("Only magne");
      break;
    case 'r':
      stabilizzatore.q0=1;
      stabilizzatore.q1 = stabilizzatore.q2 = stabilizzatore.q3 = 0;
      stabilizzatore2.reset();
//      Serial.println("Only ac");
      break;
  }
  
  if (command == 'B' || command == 'G'){
    (*sensori[0]).update();
    if ( (*sensori[0]).isUpdated() ){
      upGiro+=1;
      if (dataSensor.giroX != dataCopy.giroX || dataSensor.giroY != dataCopy.giroY || dataSensor.giroZ != dataCopy.giroZ){
        realUpGiro+=1;
        dataCopy.giroX = dataSensor.giroX;
        dataCopy.giroY = dataSensor.giroY;
        dataCopy.giroZ = dataSensor.giroZ;
      }
/*
    Serial.print("Giro ");
    Serial.print(dataSensor.giroX);
    Serial.print(' ');
    Serial.print(dataSensor.giroY);
    Serial.print(' ');
    Serial.print(dataSensor.giroZ);
    Serial.println();    
    Serial.flush();
*/
    }else{
      dataSensor.giroX = -1;
      dataSensor.giroY = -1;
      dataSensor.giroZ = -1;
    }
  }
  
  if (command == 'B' || command == 'A'){
    (*sensori[1]).update();
    if ( (*sensori[1]).isUpdated() ){
      upAcc+=1;
      if (dataSensor.accX != dataCopy.accX || dataSensor.accY != dataCopy.accY || dataSensor.accZ != dataCopy.accZ){
        realUpAcc+=1;
        dataCopy.accX = dataSensor.accX;
        dataCopy.accY = dataSensor.accY;
        dataCopy.accZ = dataSensor.accZ;
      }
/*      
    Serial.print("Acc ");
    Serial.print(dataSensor.accX);
    Serial.print(' ');
    Serial.print(dataSensor.accY);
    Serial.print(' ');
    Serial.print(dataSensor.accZ);
    Serial.println();    
    Serial.flush();    
*/
    }else{
      dataSensor.accX = 0;
      dataSensor.accY = 0;
      dataSensor.accZ = 0;
    }
  }
  

  if (command == 'B' || command == 'M'){
    (*sensori[2]).update();
    if ( (*sensori[2]).isUpdated() ){
      upMagne+=1;
      if (dataSensor.magneX != dataCopy.magneX || dataSensor.magneY != dataCopy.magneY || dataSensor.magneZ != dataCopy.magneZ){
        realUpMagne+=1;
        dataCopy.magneX = dataSensor.magneX;
        dataCopy.magneY = dataSensor.magneY;
        dataCopy.magneZ = dataSensor.magneZ;
      }
/*
    Serial.print("Magne ");
    Serial.print(dataSensor.magneX);
    Serial.print(' ');
    Serial.print(dataSensor.magneY);
    Serial.print(' ');
    Serial.print(dataSensor.magneZ);
    Serial.println();    
    Serial.flush();
*/
    }else{
      dataSensor.magneX = 0;
      dataSensor.magneY = 0;
      dataSensor.magneZ = 0;
    }
  }
  
  tmpLoopTime = micros();
  //stabilizzatore.DCMupdate(dataSensor, (tmpLoopTime-loopTime)/2000000.0);
  loopTime = tmpLoopTime;
  /*
  Serial.println("magne!");
      Serial.print(dataSensor.magneX);
      Serial.print(":");
      Serial.print(dataSensor.magneY);
      Serial.print(":");
      Serial.print(dataSensor.magneZ);
      Serial.println();
      */
  stabilizzatore2.update(dataSensor);

  tempMillis = millis()-lastMillisDebugIMU;
  if (tempMillis >= 100){
    /*
    float e[3];
    stabilizzatore.fromQuaternionToEuler(e);
    Serial.println("Angoli attuali");
    Serial.print(e[0]);
    Serial.print(" ");
    Serial.print(e[1]);
    Serial.print(" ");
    Serial.print(e[2]);
    Serial.print(" ");
    Serial.println();
    */
    stabilizzatore2.printQuad();
 /*   
    Serial.println("Quat");
    Serial.print(stabilizzatore.q0);
    Serial.print(" ");
    Serial.print(stabilizzatore.q1);
    Serial.print(" ");
    Serial.print(stabilizzatore.q2);
    Serial.print(" ");
    Serial.print(stabilizzatore.q3);
    Serial.println();
    Serial.flush();
   */ 
    lastMillisDebugIMU = millis();
  }

  tempMillis = millis()-lastMillis;
  if (tempMillis >= 1000){
    Serial.print("Free ram: ");
    Serial.println(freeMemory());
    Serial.print("UPDATE info: gyro");
    Serial.print(upGiro);
    Serial.print(" ");
    Serial.print(realUpGiro);
    Serial.print(" acc");
    Serial.print(upAcc);
    Serial.print(" ");    
    Serial.print(realUpAcc);
    Serial.print(" magne");    
    Serial.print(upMagne);
    Serial.print(" ");    
    Serial.print(realUpMagne);    
    Serial.print(" ");
    Serial.print(tempMillis);
    Serial.println();
    Serial.flush();
    upGiro=0;
    realUpGiro=0;
    upAcc=0;
    realUpAcc=0;
    upMagne=0;
    realUpMagne=0;
    lastMillis = millis();
  }
  
  //Serial.println(); 
}


