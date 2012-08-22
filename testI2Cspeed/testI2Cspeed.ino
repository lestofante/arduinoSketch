#include "I2cMaster.h"

TwiMaster rtc = TwiMaster(false);

#define ITG3200  0x68

void setup(void) {
  Serial.begin(9600);
  Serial.println("ok1");
  writeByteITG3200(0x3E,0x80); //reset!
  writeByteITG3200(0x15,0x00); //sample rate dividere = 0
  writeByteITG3200(0x16,0x18); //+-2000°/s and 8kHz sample
  Serial.println("ok2");
}

long count;
unsigned long lastMillis = millis(), tempMillis;
uint8_t r[6];
uint8_t err;

void loop(void) {
  err = readITG3200(0x1D, r, 6);
  if ( err != 0 ){
    Serial.print(err);
    Serial.println(" error");
  }else
    count ++;
    
  tempMillis = millis()-lastMillis;
  if (tempMillis >= 1000){ 
    Serial.println(count);
    count=0;
    lastMillis = millis();
  }
   
}

uint8_t readITG3200(uint8_t address, uint8_t *buf, uint8_t count) {
  // issue a start condition, send device address and write direction bit
    if (!rtc.start(ITG3200 | I2C_WRITE)) return false;

  // send the DS1307 address
  if (!rtc.write(address)) return false;

  // issue a repeated start condition, send device address and read direction bit
  if (!rtc.restart(ITG3200 | I2C_READ))return false;

  // read data from the DS1307
  for (uint8_t i = 0; i < count; i++) {
    // send Ack until last byte then send Ack
    buf[i] = rtc.read(i == (count-1));
  }

  // issue a stop condition
  rtc.stop();
  return true;
}


uint8_t writeByteITG3200(uint8_t address, uint8_t buf){
  // issue a start condition, send device address and write direction bit
    if (!rtc.start(ITG3200 | I2C_WRITE)) return false;

  // send the DS1307 address
  if (!rtc.write(address)) return false;

  // send data to the DS1307
  if (!rtc.write(buf) ) return false;

  // issue a stop condition
  rtc.stop();
  return true;
}
//------------------------------------------------------------------------------

/*

 * write 'count' bytes to DS1307 starting at 'address'
 
 */

uint8_t writeITG3200(uint8_t address, uint8_t *buf, uint8_t count) {
  // issue a start condition, send device address and write direction bit
    if (!rtc.start(ITG3200 | I2C_WRITE)) return false;

  // send the DS1307 address
  if (!rtc.write(address)) return false;

  // send data to the DS1307
  for (uint8_t i = 0; i < count; i++) {
    if (!rtc.write(buf[i])) return false;
  }

  // issue a stop condition
  rtc.stop();
  return true;
}

/*
#include "I2C.h"
 
 #define ITG3200  0x68 
 
 //I2C I2c;
 
 void setup(void) {
 Serial.begin(9600);
 I2c.pullup(0); //no pullup
 I2c.setSpeed(1); //fast I2C
 I2c.begin();
 
 I2c.write(ITG3200,0x3E,0x80); //reset!
 I2c.write(ITG3200,0x15,0x00); //sample rate dividere = 0
 I2c.write(ITG3200,0x16,0x18); //+-2000°/s and 8kHz sample
 
 
 }
 int x, y, z;
 long count;
 unsigned long lastMillis = millis(), tempMillis;
 void loop(){
 I2c.read(ITG3200,0x1D,6);
 x = I2c.receive() << 8;
 x |= I2c.receive();
 y = I2c.receive() << 8;
 y |= I2c.receive();
 z = I2c.receive() << 8;
 z |= I2c.receive(); 
 count ++;
 tempMillis = millis()-lastMillis;
 if (tempMillis >= 1000){
 Serial.println(count);
 count=0;
 lastMillis = millis();
 }
 }
 */

