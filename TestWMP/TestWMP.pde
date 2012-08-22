#include "WMP.h"
//#include "Stabilizzazione.h"

#define DEBUG_ORIZON_TIME_MICROS 20000L //50 fps
WMP sen;
//Stabilizzazione oriz;

double angle[3];
void setup(){
  Serial.begin(19200);
  Serial.print("Inizio1:");
  sen.init();
  Serial.print("Inizio2:");
  //oriz.start();
  
  for (int i=0;i<3;i++){
    angle[i]=0;
  }
}

unsigned long timeLastDebugOrizon = 0;
unsigned long actualTimeMicros = 0;
int l=0;
long loopCount=0;
unsigned long timeDebug=millis();
unsigned long timeUp = micros();
unsigned long start = micros();
float *result;
//double angle =0;

void loop(){
  loopCount++;
  //  if ( last+1<millis() ){
  //    last=millis();
  unsigned long time = actualTimeMicros = micros();
  if ( sen.update() ){
    l++;
    timeUp+=micros()-time;
    result = sen.leggi();
//    angle=oriz.update(result[2]*0.0174532925*8, -result[1]*0.0174532925*8, result[0]*0.0174532925*8, 0,0,1);
    for (int i=0;i<3;i++){
      angle[i]+=result[i];//*0.0174532925*8;
    }
  }

  if (actualTimeMicros-timeLastDebugOrizon>DEBUG_ORIZON_TIME_MICROS){
    for (int i=0;i<3;i++){
      Serial.print(angle[i], 2);
      Serial.print("\t\t");
    }
    Serial.println();
  //if (loopNumber > DEBUG_TIME){
    // print the orizon quaternion
/*
    Serial.print(oriz.q0);
    Serial.print(" ");
    Serial.print(oriz.q1);
    Serial.print(" ");
    Serial.print(oriz.q2);
    Serial.print(" ");
    Serial.print(oriz.q3);
    Serial.println();
*/    
    timeLastDebugOrizon=micros();
  }
  //  }
  if ( timeDebug+1000<millis() ){

    Serial.print("Valori: ");
    for (int i=0;i<3;i++){
      Serial.print(" ");
      Serial.print(result[i]);
    }
    Serial.println();
    Serial.print("read per second:");
    Serial.println(l);
    Serial.print("loop second:");
    Serial.println(loopCount);
    Serial.print("running seconds:");
    Serial.println( (millis()-start)/1000 );

    Serial.print("time update:");
    Serial.println( timeUp/l );
 //   oriz.setTime( timeUp/l );
    timeUp=0;

    sen.debug();

    Serial.println();
    loopCount=l=0;
    timeDebug=millis();
    
//    sen.leggi();
//    oriz.debug();
  }
}


