#include "ADXL345.h"

void ADXL345::setup(struct data *d){
  address = 0x53;
  store = d;
  isUp=false;
 
  //reset
  /*
  Wire.beginTransmission(address);
  Wire.write(0x2D);  
  Wire.write(0x00);
  Wire.endTransmission();
  */
 /*
  //sleep
  Wire.beginTransmission(address);
  Wire.write(0x2D);  
  Wire.write(0x0f);
  Wire.endTransmission();
 */
  //IMPORTANT!!!! THIS WILL WAKE UP ADXL!!!!
  Wire.beginTransmission(address);
  Wire.write(0x2D);  
  Wire.write(0x08);
  Wire.endTransmission();
  
  //set frequency output at 800Hz
  Wire.beginTransmission(address);
  Wire.write(0x2C);  
  Wire.write(0x0D);
  Wire.endTransmission();
  
}

void ADXL345::calibrate(){
  update();
  midX = x;
  midY = y;
  midZ = z;
  for (int count=0; count < 1000; count++){
    update();
    midX = (midX+x)/2;
    midY = (midY+y)/2;
    midZ = (midZ+z)/2;
  }
  
  Serial.print("acc mid tmp ");
  Serial.print(midX);
  Serial.print(" ");
  Serial.print(midY);
  Serial.print(" ");
  Serial.print(midZ);
  Serial.print(" ");
  
  g = midZ - (midX+midY)/2;
  midZ = (midX+midY)/2;
  
  (*store).accX=0;
  (*store).accY=0;
  (*store).accZ=0;
}

void ADXL345::update(){
  Wire.beginTransmission(address);
  Wire.write(0x32); //select register 3, X MSB register
  Wire.endTransmission();
  
  Wire.requestFrom(address, 6);

  while(Wire.available()<6){
  //  Serial.print("available: ");
  //  Serial.println(Wire.available(), DEC);
  //  delay(250);
  }
  
  if(6<=Wire.available()){
    int x1, y1, z1;
    x1 = Wire.read();
    x = Wire.read()<<8; //X msb
    x |= x1; //X lsb
    
    y1 = Wire.read();
    y = Wire.read()<<8; //Z msb
    y |= y1; //Z lsb
    
    z1 = Wire.read();
    z = Wire.read()<<8; //Y msb
    z |= z1; //Y lsb
    
    //float sum = x+y+z;
    
    isUp=true;
    (*store).accX=x-midX;
    (*store).accY=y-midY;
    (*store).accZ=(z-midZ)*-1;
  }
}

boolean ADXL345::isUpdated(){
    if (isUp){
    isUp=false;
    return true;
  }
  return false;
}
void ADXL345::reset(){
}
void ADXL345::debug(){
  Serial.print("acc mid ");
  Serial.print(midX);
  Serial.print(" ");
  Serial.print(midY);
  Serial.print(" ");
  Serial.print(midZ);
  Serial.print(" ");
  Serial.print(g);
  Serial.println();
}
