#include "LestoCommWriter.h"

struct buffer{
  byte index;
  uint8_t buf[bufferSize]; //we restart buffer if it is full!
}output;
uint8_t _newbit, _changed, _oldbit;

void LestoCommWriter::init(){
  pinMode (12, OUTPUT); //PCINT4 output
  pinMode (13, INPUT); //PCINT5 clock

  output.index=0;
  
  clockStatus=0;

  // pin change interrupt FROM PCINT0 to PCINT7 (PCIE0)
  PCICR |= (1 << PCIE0);

  _newbit=_oldbit=PINB;

  // enable pin 12 (PCINT5) to change interrupt. Don't modify other interrupt, for compatibility.
  PCMSK0 = PCMSK0 | (1<<PCINT5);
  
}

SIGNAL(PCINT0_vect) {
  _newbit=PINB;
  _changed=_newbit^_oldbit;
  
  if (_changed&PCINT5){
    if (ouput.index <= 0){
      digitalWrite(12, LOW);
    }else{
      digitalWrite(12, output.buf[--output.index]);
    }
  }
  
  _oldbit=_newbit;
}

void LestoCommWriter::writeByte(uint8_t in){
  if (ouput.index < bufferSize){
    output.buf[ouput.index++] = in;
    return true;
  }
  return false;
}
