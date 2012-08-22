#include <EEPROM.h>

unsigned long lastSignal;
unsigned long newSignal;

byte valueWritten=1;

void setup(){
  Serial.begin(9600);
  int location=0;
  byte value = EEPROM.read(location);
  Serial.print("valori da leggere:");
  Serial.println(value, DEC);
  if (value != 255)
    for (location = 1; location < value; location++){
      Serial.print( EEPROM.read(location) );
    }
  
  volatile unsigned long timer0_overflow_count ;
  Serial.print("valore var:");
  Serial.println(timer0_overflow_count);
  
  lastSignal = micros();
}

void loop(){
  newSignal = micros();
  if (newSignal%1000000==0){
    Serial.print("tick");
    Serial.print(newSignal);
  }
  if (lastSignal > newSignal){
    Serial.print("catch overflow:");
    Serial.print(lastSignal);
    Serial.print(" ");
    Serial.print(newSignal);
    int resto;
    if (valueWritten < 200){
      while (lastSignal!=0){
        resto = lastSignal%10;
        lastSignal/=10;
        EEPROM.write(valueWritten, lastSignal+'0');
        valueWritten++;
      }
      EEPROM.write(valueWritten, '\n');
      valueWritten++;
      EEPROM.write(0, valueWritten);
    }
  }
  lastSignal=newSignal;
}
