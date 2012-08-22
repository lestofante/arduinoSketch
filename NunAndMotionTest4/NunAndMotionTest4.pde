#include <Wire.h>
#include "WMPandNun.h"

WMPandNun sen;
void setup(){
  Serial.begin(19200);
  sen.init();
}

int l=0;
long loopCount=0;
unsigned long start=millis();
unsigned long t=millis();
unsigned long last=millis();
void loop(){
  loopCount++;
//  if ( last+1<millis() ){
//    last=millis();
    if ( sen.update() )
      l++;
//  }
  if ( t+1000<millis() ){
    Serial.print("read per second:");
    Serial.println(l);
    Serial.print("loop second:");
    Serial.println(loopCount);
    Serial.print("running seconds:");
    Serial.println( (millis()-start)/1000 );
    sen.debug();
    
    Serial.println();
    loopCount=l=0;
    t=millis();
  }
}
