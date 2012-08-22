#include <Wire.h>
#include "NunchukFarlocco.h"

#define ledPin 13

const int sizeFloat=sizeof(float);

NunchukFarlocco nun;
unsigned long time;
void setup(){
  //Wire.begin(2);		    // join i2c bus with address #2
  Wire.onRequest(requestEvent); // register event
  Serial.begin(19200);
  digitalWrite(ledPin, HIGH);

  nun.nunchuck_init(); // send the initilization handshake
  time = micros();
}

boolean statoLedPin=true;
int loopNumber=0;
int updateNumber=0;
unsigned long time2;

uint8_t *bufferI2C;
int count =0;
boolean setting=true;
void loop(){
  if ( nun.update() ){
    updateNumber++;
  }
  float *re = nun.leggi();
  setting=true;
  bufferI2C = (uint8_t*)re;
  setting=false;
  /*
  count=0;
  for (int i=0;i<3;i++){
    for (int a=0; a<sizeFloat;a++){
      swapBufferI2C[count]=re[i]>>(8*a);
    }
  }
  */
  
  
  loopNumber++;

  time2=micros();
  if (time2-time > 1000000L){
    time = time2-time;
    float fps = (float)loopNumber/(time/1000000.0);
    Serial.print("Loop per secondo: ");
    Serial.println(fps);
            
    fps = (float)updateNumber/(time/1000000.0);            
    
    Serial.print("Update per secondo: ");
    Serial.println(fps);
    
    float *re = nun.leggi();
    for (int i=0;i<3;i++){
      Serial.print(re[i]);
      Serial.print(" ");
    }
    Serial.println();    
    statoLedPin=!statoLedPin;
    if (statoLedPin)
      digitalWrite(ledPin, HIGH);
    else
      digitalWrite(ledPin, LOW);
      
    loopNumber=0;
    updateNumber=0;
    time=micros();
  }

}

void requestEvent(){
  /*
  float *re = nun.leggi();
  for (int i=0;i<3;i++){
    Wire.send(re[i]);
  }
  */
  Serial.print("incoming Request: ");
  Serial.println(setting?"discard":"ok");
  if (setting){
    //no data available
    for (int i = 0; i < sizeFloat*3; i++){
      Wire.send(0);
    }
  }else{
    for (int i = 0; i < sizeFloat*3; i++){
      Wire.send(bufferI2C[i]);
    }
  }
}
