#include "InputPin.h"
#include "../Utils/Utils.h"
/*
Let You read digital pin 2,4,5,6
FROM BIRONPILOT V59, tnx to ciskje
*/

//#define MAXUNSIGNEDLONGCVALUE 4294967295
#define MASKPCINT0 (1<<2)
#define MASKPCINT1 (1<<4)
#define MASKPCINT2 (1<<5)
#define MASKPCINT3 (1<<6)


//define necessary values
#define MIN_RADIO_VALUE 1100
#define MAX_RADIO_VALUE 1950
#define maxReceiverAngle 0.5f
//maxReceiverAngle is in radiant

Utils utils;

InputPin::InputPin(){
  pinMode(2, INPUT); // 3 is used for esc
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  // interrupt on pin change FROM PCINT16 to PCINT23
  PCICR |= (1 << PCIE2);

  PCMSK2 = (1 << PCINT18) | // digital pin2
  (1 << PCINT20) | // digital pin4
  (1 << PCINT21) | // digital pin5
  (1 << PCINT22); // digital pin6
}

//putted here because interrupt dosn't want it in .h
unsigned long _startIn[4];
unsigned long _rawIn[4];
unsigned long _time;
byte _oldbit=0, _newbit, _changed;
volatile boolean _hasChanged=false;

ISR(PCINT2_vect) {
  _time=micros();
  
  _newbit=PIND;
  _changed=_newbit^_oldbit;
  
  if (_changed&MASKPCINT0){//if digital PIN2 has changed

    if (_newbit&MASKPCINT0) { //if PIN2 now is high
      _startIn[0]=_time;
    }else{
      _rawIn[0]=_time-_startIn[0];
    }
    _hasChanged=true;
  }

  if (_changed&MASKPCINT1){//if digital PIN4 has changed
    if (_newbit&MASKPCINT1) 
      _startIn[1]=_time;
    else 
      _rawIn[1]=_time-_startIn[1];
    _hasChanged=true;
  }
  
  if (_changed&MASKPCINT2){//if digital PIN5 has changed
    if (_newbit&MASKPCINT2) 
      _startIn[2]=_time;
    else 
      _rawIn[2]=_time-_startIn[2];
    _hasChanged=true;
  }

  if (_changed&MASKPCINT3){//if digital PIN6 has changed
    if (_newbit&MASKPCINT3) 
      _startIn[3]=_time;
    else 
      _rawIn[3]=_time-_startIn[3];
  }
  _oldbit=_newbit;
}

bool InputPin::getAndSetFalseHasChanged(){
  bool temp = _hasChanged;
  _hasChanged = false;
  return temp;
}

int InputPin::getDuration(int i){
  return _rawIn[i];
}

int InputPin::getDuration(int i, int minValue, int maxValue){
  return map(_rawIn[i] , MIN_RADIO_VALUE, MAX_RADIO_VALUE, minValue, maxValue );
}

float InputPin::getAngle(int i){
  return utils.mapFloat( _rawIn[i], MIN_RADIO_VALUE, MAX_RADIO_VALUE, -maxReceiverAngle, maxReceiverAngle );
}
