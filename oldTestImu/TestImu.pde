#include "Stabilizzazione.h"

#define DEBUG_ORIZON_TIME_MICROS 2000000L //2 seconds
#define QUATERNION_TIME_MICROS 20000L //50 fps

Stabilizzazione orizzonte;

unsigned long timeLastLoop = micros();
unsigned long timeLastQuat = micros();
unsigned long actualTimeMicros = 0;
unsigned long timeLastDebugOrizon = 0;
unsigned long hzCount = millis();

void setup(){
  Serial.begin(57600);
  
  delay(2000);
  Serial.println("INITILIZING ORIZ");  

  orizzonte.start();

  Serial.println("DONE SETUP");

  timeLastLoop=micros();
}

unsigned long timeLastUpdate = micros();
unsigned long realHzCount=0;
long count=0;
void loop(){
  count++;
  actualTimeMicros = micros();
//  durationLoop = actualTimeMicros-timeLastLoop;
  timeLastLoop=actualTimeMicros;
  
  if (micros()-7000>=timeLastUpdate){
    unsigned long durationLoop=micros()-timeLastUpdate;
    orizzonte.setTime(durationLoop);
    orizzonte.update(durationLoop);
    realHzCount++;
    timeLastUpdate=micros();
  }
  
  readSerial();
  
  if (millis()-hzCount>=1000){
    Serial.print("LOOP Hz:");
    Serial.println(count/((millis()-hzCount)/1000) );
    count=0;
    
    Serial.print("Real Hz:");
    Serial.println(realHzCount/((millis()-hzCount)/1000) );
    realHzCount=0;
    hzCount=millis();
  }
  
  if (actualTimeMicros-timeLastQuat>QUATERNION_TIME_MICROS){
  //if (loopNumber > DEBUG_TIME){
    // print the orizon quaternion
    Serial.print(orizzonte.q0);
    Serial.print(" ");
    Serial.print(orizzonte.q1);
    Serial.print(" ");
    Serial.print(orizzonte.q2);
    Serial.print(" ");
    Serial.print(orizzonte.q3);
    Serial.println();
    
    timeLastQuat = micros();
  }
  
  if (actualTimeMicros-timeLastDebugOrizon>DEBUG_ORIZON_TIME_MICROS){   
    //orizzonte.debug();
    orizzonte.basicDebug();

    timeLastDebugOrizon=micros();
  }
}

void readSerial(){
  if (Serial.available() > 0){
    switch( Serial.read() ){
      case 'A':
        orizzonte.setAcc(true);
        orizzonte.setGyro(true);
        Serial.println("ok gyro+acc");
        break;
      case 'a':
        orizzonte.setAcc(true);
        orizzonte.setGyro(false);
        Serial.println("ok only acc");
        break;
      case 'g':
        orizzonte.setAcc(false);
        orizzonte.setGyro(true);
        Serial.println("ok only gyro");
        break;
      case 'r':
        orizzonte.reset();
        Serial.println("ok reset");
        break;
      case 'p':
        {
        float value = readRawFloat();
        orizzonte.setKp(value);
        Serial.print("ok set P to:");
        Serial.println(value);
        break;
        }
      case 'i':
        {
        float value = readRawFloat();
        orizzonte.setKi(value);
        Serial.print("ok set I to:");
        Serial.println(value);
        break;
        }
        break;
    }
  }
}

float readRawFloat(){
  union u_tag {
    byte b[4];
    float fval;
  } u;
  for (int i=0;i<4;i++){
    do{
      u.b[i]=Serial.read();
    }while(u.b[i]==-1);
  }
  return u.fval;
}
