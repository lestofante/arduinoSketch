#include "HMC5883L.h"

void HMC5883L::setup(struct data *d){
  address = 0x1E;
  store = d;
  isUp=false;
  
  //set output at 75Hz (should be 160Hz) and 8 averange
  byte zero = 0x00;
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x78);
  Wire.endTransmission(); 
  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(zero); //continuous measurement mode
  Wire.endTransmission();
  
  scaleX = 1;
  scaleY = 1;
  scaleZ = 1;
}

void HMC5883L::calibrate(){
  long midX, midY, midZ;
  long pX, pY, pZ;
  long nX, nY, nZ;
  byte zero = 0x00;
  //Serial.println("positive bias");  
  //set output at 75Hz (should be 160Hz) and no averange and positive bias
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x19);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  update(); //remove fisrt read, it has wrong setting value
  midX = midY = midZ = 0;
  int numSample = 100;
  for (int i=0; i < numSample; i++){
      //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //single measurement mode
    Wire.endTransmission();
    update();
    midX += x;
    midY += y;
    midZ += z;
  }
  
  Serial.print(midX/100);
  Serial.print(":");
  Serial.print(midY/100);
  Serial.print(":");
  Serial.println(midZ/100);
  
  pX = midX/numSample;
  pY = midY/numSample;
  pZ = midZ/numSample;
  
  //Serial.println("negative bias");
  //set output at 75Hz (should be 160Hz) and no averange and negative bias
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x1A);
  Wire.endTransmission();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x01); //single measurement mode
  Wire.endTransmission();
  update();
  midX = midY = midZ = 0;
  for (int i=0; i < 100; i++){
      //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(address); //open communication with HMC5883
    Wire.write(0x02); //select mode register
    Wire.write(0x01); //single measurement mode
    Wire.endTransmission();
    update();
    midX += x;
    midY += y;
    midZ += z;
  }
  
  Serial.print(midX/100);
  Serial.print(":");
  Serial.print(midY/100);
  Serial.print(":");
  Serial.println(midZ/100);
  
  
  nX = midX/100;
  nY = midY/100;
  nZ = midZ/100;
  
  float minV = min(pX-nX, min(pY-nY, pZ-nZ));
  scaleX = ( (pX-nX)/minV );
  scaleY = ( (pY-nY)/minV );
  scaleZ = ( (pZ-nZ)/minV );
  /*
  Serial.println( (pX-nX)/ scaleX );
  Serial.println( (pY-nY)/ scaleY );
  Serial.println( (pZ-nZ)/ scaleZ );
  */
  Serial.print("magne mid tmp ");
  Serial.print(scaleX);
  Serial.print(" ");
  Serial.print(scaleY);
  Serial.print(" ");
  Serial.print(scaleZ);
  Serial.print(" ");
  
  Wire.beginTransmission(address);
  Wire.write(zero);  
  Wire.write(0x78);
  Wire.endTransmission(); 
  
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(zero); //continuous measurement mode
  Wire.endTransmission();
  //g = midZ - (midX+midY)/2;
  //midZ = (midX+midY)/2;
  (*store).magneY=0;
  (*store).magneX=0;
  (*store).magneZ=0;
}

void HMC5883L::update(){
  
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
    
  Wire.requestFrom(address, 6);

  while(Wire.available()<6){
  //  Serial.print("available: ");
  //  Serial.println(Wire.available(), DEC);
  //  delay(250);
  }
  
  if(6<=Wire.available()){
    x = Wire.read()<<8; //X msb
    x |= Wire.read(); //X lsb
    z = Wire.read()<<8; //Z msb
    z |= Wire.read(); //Z lsb
    y = Wire.read()<<8; //Y msb
    y |= Wire.read(); //Y lsb
    
//    float sum = x+y+z;
    
    isUp=true;
    (*store).magneY=x*scaleX;
    (*store).magneX=y*scaleY;
    (*store).magneZ=z*scaleZ;
  }
}

boolean HMC5883L::isUpdated(){
    if (isUp){
    isUp=false;
    return true;
  }
  return false;
}
void HMC5883L::reset(){
}
void HMC5883L::debug(){/*
  Serial.print("magne mid ");
  Serial.print(midX);
  Serial.print(" ");
  Serial.print(midY);
  Serial.print(" ");
  Serial.print(midZ);
  Serial.print(" ");*/
  Serial.print(g);
  Serial.println();
}
