#include "Gyro.h"
#define gyro_address 0xD0

void Sensor::init(){
/*
  Wire.beginTransmission(gyro_address);
  Wire.send(0x3E);
  Wire.send(0x80);  //send a reset to the device
  Wire.endTransmission(); //end transmission
*//*
  Wire.beginTransmission(gyro_address);
  Wire.write( 0x15 );
  Wire.write( byte(0x00) );   //sample rate divider to 0
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(gyro_address);
  Wire.write(0x16);
  Wire.write(0x18); // ±2000 degrees/s 8kHz sample rate(default value)
  Wire.endTransmission(); //end transmission
/*
  Wire.beginTransmission(gyro_address);
  Wire.send(0x17);
  Wire.send(0x01);   // enable interrupt generator
  Wire.endTransmission(); //end transmission
*/
}

void Sensor::update(){
  Serial.println("ind");
  Wire.beginTransmission(gyro_address+1);
  Wire.write(0x1D);
  Wire.endTransmission();

  Serial.println("request");  
  Wire.requestFrom(gyro_address+1, 6);
  int readed=0;
  while(readed<6)    // slave may send less than requested
  { 
    if (Wire.available()){
      char c = Wire.read(); // receive a byte as character
      Serial.print("value:");
      Serial.println(c);         // print the character
      readed++;
    }
  }
  Wire.endTransmission();
}

boolean Sensor::isDataUpdated(){
  return false;
}

void Sensor::reset(){
}

void Sensor::debug(){
}



