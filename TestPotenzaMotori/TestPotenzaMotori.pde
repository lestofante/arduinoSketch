#include <Servo.h>
#define PIN_MOTORE 3

Servo motore = Servo();

void setup(){
  Serial.begin(19200);
  motore.attach( PIN_MOTORE, 1000, 2500 );
}

int enginePower=0;
unsigned long lastPrint=0;
void loop(){
  char letto='n';
  boolean lettoQualcosa=false;
  while (Serial.available() > 0){
    letto = Serial.read();
    lettoQualcosa=true;
    switch(letto){
      case 'n':
        break;
      case '-':
        enginePower--;
        motore.write(enginePower);
        break;
      case '+':
        enginePower++;
        motore.write(enginePower);
        break;
      case 's':
        enginePower=0;
        motore.write(enginePower);
        break;
    }
  }
  if (lettoQualcosa){
    Serial.print("new power: ");
    Serial.println(enginePower);
  }
  if (millis() - lastPrint > 1000){
    lastPrint=millis();
    Serial.println(enginePower);
  }
}
