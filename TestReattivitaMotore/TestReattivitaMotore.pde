#include <Servo.h>
#include <Wire.h>
//#include "NunchukFarlocco.h"

#define PIN_MOTORE 3
#define MAX_VALUES 70
Servo motore = Servo();
//NunchukFarlocco nun;
boolean continua=true;
unsigned long startRead=0;

void setup(){
  Serial.begin(19200);
  motore.attach( PIN_MOTORE, 1000, 2500 );
//  nun.nunchuck_init(); // send the initilization handshake
  motore.write(0);
  delay(3000);
  startRead=micros();
}

float values[MAX_VALUES];
long valueTime[MAX_VALUES];
int enginePower=0;
unsigned long lastPrint=0;

int actualValue=0;

void loop(){
//  delay(2);
/*
  nun.update();
  values[actualValue]=nun.value[2];
  
  //if (values[actualValue]!=0){
    valueTime[actualValue]= micros()-startRead;
    actualValue++;
  //}else{
//   Serial.println(); 
  //}
  /*
  float *ris=nun.leggi();
  for (int i=0;i<3;i++){
    values[actualValue][i]=ris[i];
  }
  */
  
  if (actualValue>=MAX_VALUES){
    actualValue=0;
    printValues();
    startRead=micros();
  }

  char letto='n';
  boolean lettoQualcosa=false;
  while (Serial.available() > 0){
    letto = Serial.read();
    lettoQualcosa=true;
    switch(letto){
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
      case 'p':
        actualValue=0;
        printValues();
        resettaLettura();
        break;
    }
  }
  if (lettoQualcosa){
    Serial.print("new power: ");
      Serial.println(enginePower);
    //motore.write(enginePower);
  }
  /*
  if (millis() - lastPrint > 1000){
    lastPrint=millis();
    Serial.println(enginePower);
  }
  */
}

void resettaLettura(){
  actualValue=0;
  startRead=micros();
}

void printValues(){
  unsigned long iniziostampa=micros();
  
  Serial.println("Dati raccolti:");
  for (int i=0; i < MAX_VALUES; i++){
    Serial.print(" time:");
    Serial.print(valueTime[i]);
    {
      Serial.print(" ");
      Serial.print(values[i]);
    }
    /*
    for (int a=0; a < MAX_VALUES; a++){
      Serial.print(" ");
      Serial.print(values[i][a]);
    }
    */
    Serial.println();
  }
  Serial.print(" time stampa:");
  Serial.println(micros()-iniziostampa);
  Serial.println("*");
  
  if (continua){
    enginePower++;
  }
  if (enginePower>100){
    enginePower=0;
    continua = false;
  }
  Serial.print("new power: ");
  Serial.println(enginePower);
  resettaLettura();
  motore.write(enginePower);
}

