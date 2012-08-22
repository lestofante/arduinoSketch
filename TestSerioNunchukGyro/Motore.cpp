#import "Motore.h"

#define ENGINEMIN 20
#define ENGINEMAX 230

Motore::Motore(int pin){
  PWMpin=pin;
}

void Motore::update(int actualPower){
  actualPower=constrain(map(actualPower, 0, 255, ENGINEMIN, ENGINEMAX), ENGINEMIN, ENGINEMAX);
  analogWrite(PWMpin, actualPower);
}
