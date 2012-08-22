#include <Wire.h>
#include "NunchukFarlocco.h"

#define ledPin 13

NunchukFarlocco nun;
unsigned long time;
void setup(){
  Serial.begin(115200);
  digitalWrite(ledPin, HIGH);

  nun.nunchuck_init(); // send the initilization handshake
  time = micros();
}

boolean statoLedPin=true;
int loopNumber=0;
int updateNumber=0;
unsigned long time2;
void loop(){
  if ( nun.update() ){
    //updateNumber++;
    float *re = nun.leggi();
    for (int i=0;i<3;i++){
      Serial.print(re[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
/*  
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
*/
}
