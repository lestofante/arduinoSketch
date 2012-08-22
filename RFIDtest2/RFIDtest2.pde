
#include <NewSoftSerial.h>

NewSoftSerial mySerial(2, 3);

void setup()  
{
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  // set the data rate for the NewSoftSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
  pinMode(8,OUTPUT);
  digitalWrite(8, HIGH);
}
int  val = 0; 
char code[14];
int bytesread = 0; 
void loop() { 

  if(mySerial.available() > 0) {            // if data available from reader 
    if((val = mySerial.read()) == 10) {     // check for header 
      bytesread = 0; 
      while(bytesread < 14) {             // read 14 digit code 
        if( mySerial.available() > 0) { 
          val = mySerial.read(); 
          if((val == 10)||(val == 13)) {  // if header or stop bytes before the 10 digit reading 
            break;                        // stop reading 
          } 
          code[bytesread] = val;          // add the digit           
          bytesread++;                    // ready to read next digit  
        } 
      } 
      if(bytesread == 14) {               // if 14 digit read is complete 
        Serial.print("TAG code is: ");    // possibly a good TAG 
        Serial.println(code);             // print the TAG code 
      } 
      bytesread = 0; 
      digitalWrite(2, LOW);               // deactivate the RFID reader for a moment so it will not flood
      delay(1500);                        // wait for a bit 
      digitalWrite(2, HIGH);              // Activate the RFID reader
    } 
  } 
} 
