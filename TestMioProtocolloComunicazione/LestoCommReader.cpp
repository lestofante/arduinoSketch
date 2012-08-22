#include "LestoCommReader.h"

void LestoCommReader::init(){
  pinMode (12, INPUT); //PCINT4 input
  pinMode (13, OUTPUT); //PCINT5 clock
}

uint8_t LestoCommReader::readByte(){
  uint8_t out=0;
  for(byte i=0; i<8;i++){
    //next clock
    digitalWrite(13, clockStatus?HIGH:LOW);
    clockStatus=!clockStatus;
    
    out+=digitalRead(12);
    out = out<<1;
  }
  retrun out;
}
