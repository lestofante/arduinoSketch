
#include <NewSoftSerial.h>

NewSoftSerial mySerial(2, 3);

void setup()  
{
  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  // set the data rate for the NewSoftSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

boolean waitingForReply=false;
String buffer;
void loop()                     // run over and over again
{

  if (mySerial.available()) {
    if (!waitingForReply)
      buffer+=(char)mySerial.read();
      if ( buffer.endsWith('\n') ){
        Serial.print((char)mySerial.read());
        waitingForReply=true;
      }
    }
  }
  if (Serial.available()) {
      //mySerial.print((char)Serial.read());
      
  }
}
