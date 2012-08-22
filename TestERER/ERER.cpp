#import "ERER.h"

/*
  ERER.cpp - Library for ERER sensors.
  (use 2 infrared LEDs to build a distance sensor)
  Rewritten by Mauro Mombelli 03/04/2011
  Based on code of Giovanni Blu Mitolo, March 11, 2011.
  Released with Creative Commons Attribution licence  
*/

volatile byte _oldbit;
byte _changed, _newbit;
volatile boolean _hasChanged=false;
volatile long rawDistance=0;
volatile byte BCDreceiverN, BCDreceiverP, BCDemitterP;
volatile byte MASKreceiverN, MASKemitterP, MASKreceiverP;
volatile boolean emitterStatus;
unsigned long _time;
unsigned long _startIn[2];
unsigned long _rawIn[2];

ERER::ERER(byte PCINTemitterP, byte PCINTreceiverP, byte PCINTreceiverN){

  //set emitterP pin mode
  BCDemitterP = getBCDpin(PCINTemitterP); //find BCD (see PortManipulation)
  MASKemitterP = (1<< (PCINTemitterP - (8*BCDemitterP) ) ); //find MASK for esy port manipulation
  //set as OUTPUT and HIGH
  switch (BCDemitterP){
    case 0:
      DDRB |= MASKemitterP;
      PORTB |= MASKemitterP;
      break;
    case 1:
      DDRC |= MASKemitterP;
      PORTC |= MASKemitterP;
      break;
    case 2:
      DDRD |= MASKemitterP;
      PORTD |= MASKemitterP;
      break;
  }
  emitterStatus=true;
  
  //set receiverP pin mode
  BCDreceiverP = getBCDpin(PCINTreceiverP); //find BCD (see PortManipulation)
  MASKreceiverP = (1<< (PCINTreceiverP - (8*BCDreceiverP) ) ); //find MASK for esy port manipulation
  //set as OUTPUT and LOW
  switch (BCDreceiverP){
    case 0:
      DDRB |= MASKreceiverP;
      PORTB &= ~MASKemitterP;
      break;
    case 1:
      DDRC |= MASKreceiverP;
      PORTC &= ~MASKemitterP;
      break;
    case 2:
      DDRD |= MASKreceiverP;
      PORTD &= ~MASKemitterP;
      break;
  }
  
  //set PCINTreceiverN as INPUT AND attach Interrupt
  /*
  remember:
  PORTB maps to Arduino digital pins 8 to 13 (PCIE0) form PCINT0 to PCINT7
  PORTC maps to Arduino analog pins 0 to 5 (PCIE1) form PCINT8 to PCINT14
  PORTD maps to Arduino digital pins 0 to 7 (PCIE2) form PCINT16 to PCINT23
  */
  BCDreceiverN = getBCDpin(PCINTreceiverN);
  MASKreceiverN = (1<< (PCINTreceiverN - (8*BCDreceiverN) ) ); //find MASK for esy port manipulation
  
  switch (BCDreceiverN){
    case 0:
      PORTB &= ~MASKreceiverN;//set as INPUT
      PCICR |= (1 << PCIE0); //INTERRUPT ON PCIE0 CHANGE ACTIVATED
      PCMSK0 |= MASKreceiverN; //UMASKING INTERRUPT FOR PIN
      _oldbit = PINB;//To understand witch pin has changed
      break;
    case 1:
      PORTC &= ~MASKreceiverN;//set as INPUT
      PCICR |= (1 << PCIE1); //INTERRUPT ON PCIE0 CHANGE ACTIVATED
      PCMSK1 |= MASKreceiverN; //UMASKING INTERRUPT FOR PIN
      _oldbit = PINC;//To understand witch pin has changed
      break;
    case 2:
      PORTD &= ~MASKreceiverN;//set as INPUT
      PCICR |= (1 << PCIE2); //INTERRUPT ON PCIE0 CHANGE ACTIVATED
      PCMSK2 |= MASKreceiverN; //UMASKING INTERRUPT FOR PIN
      _oldbit = PIND;//To understand witch pin has changed
      break;
  }
  
  //charge ReceiverN using internal pull-up
  setStatus(BCDreceiverN, MASKreceiverN, true);
  //stop charge ReceiverP
  setStatus(BCDreceiverN, MASKreceiverN, false);
}

long ERER::getRawDistance(){
  return rawDistance;
}

byte ERER::getBCDpin(byte PCINTpin){
  if (PCINTpin>=0 && PCINTpin<=7){
    return 0;
  }
  
  if (PCINTpin>=8 && PCINTpin<=14){
    return 1;
  }
  
  if (PCINTpin>=16 && PCINTpin<=23){
    return 2;
  }
  return 3;  
}

void ERER::setStatus(byte BCD, byte MASK, boolean state){
  if (state){
    switch (BCD){
      case 0:
      PORTB |= MASK;
      break;
    case 1:
      PORTC |= MASK;
      break;
    case 2:
      PORTD |= MASK;
      break;
    }
  }else{
    switch (BCD){
      case 0:
      PORTB &= ~MASK;
      break;
    case 1:
      PORTC &= ~MASK;
      break;
    case 2:
      PORTD &= ~MASK;
      break;
    }
  }
}

ISR(PCINT2_vect) {
  if (BCDreceiverN == 2){
    _newbit=PIND;
    _changed=_newbit^_oldbit;
  
    if (_changed & MASKreceiverN){//if PCINTreceiverN has changed
      _time=micros();
      _hasChanged=true;
      
      if (_newbit & MASKreceiverN) { //if PCINTreceiverN now is high
        _startIn[emitterStatus]=_time;
      }else{
        _rawIn[emitterStatus]=_time-_startIn[emitterStatus];
      
        if (!emitterStatus){ //if we read with emitter HIGH and then LOW we have a distace
          rawDistance = _rawIn[0]-_rawIn[1]; //because dark read teorically is lower than light read
        }
      
        //start a new reading changing emitter status
        emitterStatus!=emitterStatus;
        if (emitterStatus){
          switch (BCDemitterP){
            case 0:
              PORTB |= MASKemitterP;
              break;
            case 1:
              PORTC |= MASKemitterP;
              break;
            case 2:
              PORTD |= MASKemitterP;
              break;
          }
        }else{
          switch (BCDemitterP){
            case 0:
              PORTB &= ~MASKemitterP;
              break;
            case 1:
              PORTC &= ~MASKemitterP;
              break;
            case 2:
              PORTD &= ~MASKemitterP;
              break;
          }
        }
    
        //charge ReceiverN using internal pull-up
        PORTD |= MASKreceiverN; //we alreadi know it's PORTD becuse we are in this interrupt :-)
        
        //stop charge ReceiverN
        PORTD &= ~MASKreceiverN;
      }
    }
  
    _oldbit=_newbit;
  }
}

ISR(PCINT1_vect) {
  if (BCDreceiverN == 1){
    _newbit=PINC;
    _changed=_newbit^_oldbit;
  
    if (_changed & MASKreceiverN){//if PCINTreceiverN has changed
      _time=micros();
      _hasChanged=true;
      
      if (_newbit & MASKreceiverN) { //if PCINTreceiverN now is high
        _startIn[emitterStatus]=_time;
      }else{
        _rawIn[emitterStatus]=_time-_startIn[emitterStatus];
      
        if (!emitterStatus){ //if we read with emitter HIGH and then LOW we have a distace
          rawDistance = _rawIn[0]-_rawIn[1]; //because dark read teorically is lower than light read
        }
      
        //start a new reading changing emitter status
        emitterStatus!=emitterStatus;
        if (emitterStatus){
          switch (BCDemitterP){
            case 0:
              PORTB |= MASKemitterP;
              break;
            case 1:
              PORTC |= MASKemitterP;
              break;
            case 2:
              PORTD |= MASKemitterP;
              break;
          }
        }else{
          switch (BCDemitterP){
            case 0:
              PORTB &= ~MASKemitterP;
              break;
            case 1:
              PORTC &= ~MASKemitterP;
              break;
            case 2:
              PORTD &= ~MASKemitterP;
              break;
          }
        }
    
        //charge ReceiverN using internal pull-up
        PORTC |= MASKreceiverN; //we alreadi know it's PORTC becuse we are in this interrupt :-)
        
        //stop charge ReceiverN
        PORTC &= ~MASKreceiverN;
      }
    }
  
    _oldbit=_newbit;
  }
}

ISR(PCINT0_vect) {
  if (BCDreceiverN == 0){
    _newbit=PINB;
    _changed=_newbit^_oldbit;
  
    if (_changed & MASKreceiverN){//if PCINTreceiverN has changed
      _time=micros();
      _hasChanged=true;
      
      if (_newbit & MASKreceiverN) { //if PCINTreceiverN now is high
        _startIn[emitterStatus]=_time;
      }else{
        _rawIn[emitterStatus]=_time-_startIn[emitterStatus];
      
        if (!emitterStatus){ //if we read with emitter HIGH and then LOW we have a distace
          rawDistance = _rawIn[0]-_rawIn[1]; //because dark read teorically is lower than light read
        }
      
        //start a new reading changing emitter status
        emitterStatus!=emitterStatus;
        if (emitterStatus){
          switch (BCDemitterP){
            case 0:
              PORTB |= MASKemitterP;
              break;
            case 1:
              PORTC |= MASKemitterP;
              break;
            case 2:
              PORTD |= MASKemitterP;
              break;
          }
        }else{
          switch (BCDemitterP){
            case 0:
              PORTB &= ~MASKemitterP;
              break;
            case 1:
              PORTC &= ~MASKemitterP;
              break;
            case 2:
              PORTD &= ~MASKemitterP;
              break;
          }
        }
    
        //charge ReceiverN using internal pull-up
        PORTB |= MASKreceiverN; //we alreadi know it's PORTB becuse we are in this interrupt :-)
        
        //stop charge ReceiverN
        PORTB &= ~MASKreceiverN;
      }
    }
  
    _oldbit=_newbit;
  }
}
