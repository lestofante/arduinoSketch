#include "ITG3200.h"

void ITG3200::setup(struct data *d){
  address = 0x68;
  store = d;
  isUp=false;
  
  Wire.beginTransmission(address);
  Wire.write(0x3E);
  Wire.write(0x80);  //send a reset to the device
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(address);
  Wire.write( 0x15 );
  Wire.write( byte(0x00) );   //sample rate divider to 0
  Wire.endTransmission(); //end transmission

  Wire.beginTransmission(address);
  Wire.write(0x16);
  Wire.write(0x18); // ±2000 degrees/s 8kHz sample rate(default value)
  Wire.endTransmission(); //end transmission
  //938.68027
  toRadians = 938.73405;
}

void ITG3200::calibrate(){
  int midX=0, midY=0, midZ=0;
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
  (*store).giroX=-1;
  (*store).giroY=-1;
  (*store).giroZ=-1;
}

void ITG3200::update(){

  //    Serial.println("a");
  Wire.beginTransmission(address);
  Wire.write(0x1D); //select register 3, X MSB register
  Wire.endTransmission();


  Wire.requestFrom(address, 6);

  while(Wire.available()<6){
    //Serial.print("a: ");
    //Serial.println(Wire.available(), DEC);
  }

  if(6<=Wire.available()){
    //Serial.println("r1");
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    y = Wire.read()<<8; //Z msb
    y |= Wire.read(); //Z lsb
    z = Wire.read()<<8; //Y msb
    z |= Wire.read(); //Y lsb
    /*  
    Serial.print(x);
    Serial.print(':');
    Serial.print(y);
    Serial.print(':');
    Serial.print(z);
    Serial.println(); 
    */
    isUp=true;
    (*store).giroX=((x-midX)/toRadians)*-1;
    (*store).giroY=((y-midY)/toRadians)*-1;
    (*store).giroZ=((z-midZ)/toRadians)*-1;
  }


}
boolean ITG3200::isUpdated(){
  if (isUp){
    isUp=false;
    return true;
  }
  return false;
}
void ITG3200::reset(){
}
void ITG3200::debug(){
  Serial.print("giro mid");
  Serial.print(midX);
  Serial.print(" ");
  Serial.print(midY);
  Serial.print(" ");
  Serial.print(midZ);
  Serial.println();
}



