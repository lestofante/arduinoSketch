#include <math.h>

#include "Wire.h"
#include "WiiChuck.h"
//#include "nunchuck_funcs.h"

#define MAXANGLE 90
#define MINANGLE -90


WiiChuck chuck = WiiChuck();
int angleStart, currentAngle;
int tillerStart = 0;
double angle;

void setup() {
  //nunchuck_init();
  Serial.begin(115200);
  Serial.print(", "); 
  chuck.begin();
  Serial.print(", "); 
  chuck.update();
  Serial.print(", "); 
  //chuck.calibrateJoy();
}


void loop() {
  delay(200);
  chuck.update();


  Serial.print(chuck.readRoll());
    Serial.print(", ");  
  Serial.print(chuck.readPitch());
    Serial.print(", ");  

    Serial.print((int)chuck.readAccelX());
    Serial.print(", ");  
    Serial.print((int)chuck.readAccelY());
    Serial.print(", ");  

    Serial.print((int)chuck.readAccelZ());

    Serial.println();

}
