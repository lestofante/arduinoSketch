/*
An Arduino code example for interfacing with the HMC5883

by: Jordan McConnell
 SparkFun Electronics
 created on: 6/30/11
 license: OSHW 1.0, http://freedomdefined.org/OSHW

Analog input 4 I2C SDA
Analog input 5 I2C SCL
*/

#include <Wire.h> //I2C Arduino Library

#define address 0x53 //0011110b, I2C 7bit address of HMC5883

void setup(){
  //Initialize Serial and I2C communications
  Serial.begin(9600);
  delay(1000);
  Wire.begin();
  Serial.print("START ");
  
  //IMPORTANT!!!!
  //THIS WILL WAKE UP ADXL!!!!
  Wire.beginTransmission(address);
  Wire.send(0x2D);  
  Wire.send(0x08);
  Wire.endTransmission();
  
/*  
  Wire.beginTransmission(address);
  Wire.send(0x2C);  
  Wire.send(0x0F);
  Wire.endTransmission();
  
  Wire.beginTransmission(address);
  Wire.send(0x31);  
  Wire.send(0x01);
  Wire.endTransmission();
/*  
  Wire.beginTransmission(address);
  Wire.send(0x2C);  
  Wire.send(0x2C);
  Wire.endTransmission();
  /*
  Wire.beginTransmission(address);
  Wire.send(0x3E);
  Wire.send(0x80);  //send a reset to the device
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(address);
  Wire.send( 0x15 );
  Wire.send( byte(0x00) );   //sample rate divider to 0
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(address);
  Wire.send(0x16);
  Wire.send(0x18); // ±2000 degrees/s 8kHz sample rate(default value)
  Wire.endTransmission(); //end transmission
/*
  Wire.beginTransmission(gyro_address);
  Wire.send(0x17);
  Wire.send(0x01);   // enable interrupt generator
  Wire.endTransmission(); //end transmission
*/
}

void loop(){
  
  int x,y,z; //triple axis data
  Serial.print("START READING 1");
  
  Wire.beginTransmission(0x53);
  Wire.send(0x32); //select register 3, X MSB register
  Wire.endTransmission();
  
 Serial.print("START READING 2");
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(0x53, 6);
  Serial.print("available: ");
  Serial.println(Wire.available(), DEC);
  while(Wire.available()<6){
    Serial.print("available: ");
    Serial.println(Wire.available(), DEC);
    delay(250);
  }
  if(6<=Wire.available()){
    x = Wire.receive()<<8; //X msb
    x |= Wire.receive(); //X lsb
    z = Wire.receive()<<8; //Z msb
    z |= Wire.receive(); //Z lsb
    y = Wire.receive()<<8; //Y msb
    y |= Wire.receive(); //Y lsb
  }
  
  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(x);
  Serial.print("  y: ");
  Serial.print(y);
  Serial.print("  z: ");
  Serial.println(z);
  
  delay(250);
}


