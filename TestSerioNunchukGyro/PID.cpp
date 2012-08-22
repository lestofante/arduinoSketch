#include "PID.h"

PID::PID(){
  P=1;
}

PID::PID(int p){
  P=p;
}

float PID::update(float angle, float setPoint){
  return P*(setPoint-angle);
}

void PID::setP(int p){
  P=p;
}
