#include <Servo.h>
#include "ReadSerial.h"

#define motore1PWMPin 3
#define motore2PWMPin 9
#define motore3PWMPin 10
#define motore4PWMPin 11

Servo motori[4]={Servo(), Servo(), Servo(), Servo()};

ReadSerial comm;

void setup(){
  Serial.begin(19200);
  
  motori[0].attach( motore1PWMPin, 1000, 2400 );
  motori[1].attach( motore2PWMPin, 1000, 2400 );
  motori[2].attach( motore3PWMPin, 1000, 2400 );
  motori[3].attach( motore4PWMPin, 1000, 2400 );
  int i;
  for(i=0; i<4; i++){
    motori[i].write(0);
  }
  delay(2000);
}

void loop(){
  if ( comm.readSerial() ){
    char c;
    float v;
    comm.get(&c, &v);
    executeCommand(c, v);
    
    int i;
    for(i=0; i<4; i++){
      Serial.print(i);
      Serial.print(":");
      Serial.println( motori[i].read() );
    }
  }
}


void executeCommand(char c, float v){
  
  Serial.print("setting command and value ");
  Serial.print(c);
  Serial.print(" ");
  Serial.println(v);
  
  int i = c-'0';
  motori[i].write( constrain(v, 0, 180) );
}
