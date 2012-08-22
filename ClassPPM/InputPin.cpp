#include "InputPin.h"

/*
Let You read digital pin 2,4,5,6,7
based on BIRONPILOTV59, tnx to ciskje
*/

#define MAXUNSIGNEDLONGCVALUE 4294967295
#define MASKPCINT0 (1<<2)
#define MASKPCINT1 (1<<4)
#define MASKPCINT2 (1<<5)
#define MASKPCINT3 (1<<6)

volatile byte _oldbit, _newbit, _changed;

InputPin::InputPin(){
  pinMode(2, INPUT); // 3 is used for esc
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  // interrupt on pin change PCINT2
  PCICR |= (1 << PCIE2);

  PCMSK2 = (1 << PCINT18) | // pin2
  (1 << PCINT20) | // pin4
  (1 << PCINT21) | // pin5
  (1 << PCINT22); // pin6
  
  _oldbit=PIND;
}

//putted here because interrupt dosn't want it in .h
unsigned long _startIn[4];
volatile unsigned long _rawIn[4];
unsigned long _time;


ISR(PCINT2_vect) {
  _time=micros();
  
  _newbit=PIND;
  _changed=_newbit^_oldbit;
  
  if (_changed&MASKPCINT0){

    if (_newbit&MASKPCINT0) {
      _startIn[0]=_time;
    }else{
      _rawIn[0]=_time-_startIn[0];
    }
  }

  if (_changed&MASKPCINT1)
    if (_newbit&MASKPCINT1) 
      _startIn[1]=_time;
    else 
      _rawIn[1]=_time-_startIn[1];

  if (_changed&MASKPCINT2)
    if (_newbit&MASKPCINT2) 
      _startIn[2]=_time;
    else 
      _rawIn[2]=_time-_startIn[2];

  if (_changed&MASKPCINT3)
    if (_newbit&MASKPCINT3) 
      _startIn[3]=_time;
    else 
      _rawIn[3]=_time-_startIn[3];
  _oldbit=_newbit;
//  blockedRX=0;
}

int InputPin::getDuration(int i){
 // Serial.print("ok:");
//  Serial.print(rawIn[i]);
  //      Serial.println(rawIn[0]);
      // Serial.print("okIND2:");
        //Serial.println(rawIn, BIN);
  return _rawIn[i];
}
