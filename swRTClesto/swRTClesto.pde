#include "swRTC.h"

swRTC tempo;

void setup(){
  Serial.begin(57600);
  //tempo.setHourDurationMS(1800000);//double velocity
  Serial.print("Setting data:");  
  Serial.println( tempo.setDate(2011, 10, 10, 18, 28, 00) );
  tempo.startRTC();
}

void loop(){
  delay(1000);
  
  int ris=tempo.updateDate();
  Serial.print("Update status (0=ok):");  
  Serial.println( ris );

  Serial.print( tempo.now.year );
  Serial.print("-");
  Serial.print( tempo.now.month );
  Serial.print("-");
  Serial.print( tempo.now.day );
  Serial.print(" ");
  Serial.print( tempo.now.hour );
  Serial.print(":");
  Serial.print( tempo.now.minute );
  Serial.print(":");
  Serial.print( tempo.now.second );
  Serial.println();
  Serial.println();
}

