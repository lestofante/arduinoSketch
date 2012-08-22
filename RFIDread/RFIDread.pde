
#include <NewSoftSerial.h>

NewSoftSerial mySerial(2, 3);
int myID=101;
const int rfidLength=11; //cannot be 2

char actualUser[rfidLength+1];

unsigned long timeLastAction=0, timeOutMillisAction=60000; //1 minute of time out
unsigned long timeLastRFID=0, timeOutMillisRFID=500; //0.5 second of time out
unsigned long timeLastAnalog=0, timeOutMillisAnalog=1000; //1 second of time out
unsigned long lastUp=0, timeOutMillisUp=60000;

unsigned long midAnalogRead=0;
int analogReadNumber=0;

void setup()  
{
  actualUser[0]='\0';
  Serial.begin(9600);
  delay(1000);
  Serial.println("Goodnight moon!");

  // set the data rate for the NewSoftSerial port
  mySerial.begin(9600);
  //mySerial.println("Hello, world?");
}

void loop()                     // run over and over again
{ 

  if (millis() > lastUp+timeOutMillisUp){//every minute
    String ris = "*";  
    ris+=myID;
    ris+="-";//tilde
    Serial.println(ris);
    lastUp=millis();
  }

  //is someone using rfid (machine is going to be in use)?
  if (mySerial.available()>=rfidLength+2) { //+2 is the \r\n
    //yes, read it and send to server, debounce of timeOutMillisRFID
    String ris = leggiRFID();
    if ( ris!=NULL && timeLastRFID+timeOutMillisRFID < millis() ){
      Serial.println(ris); //must be println
      timeLastRFID=millis();
      midAnalogRead=0;
      analogReadNumber=0;
    }

    timeLastAction=millis();
  }

  //is the machine in use?
  if (actualUser[0]!='\0'){
    //yes, use it!


    //read force sensor
    int newReadAnag = analogRead(0);
    midAnalogRead+=newReadAnag;
    analogReadNumber++;

    if (millis() > timeLastAnalog+timeOutMillisAnalog){
      writeForce();
      timeLastAnalog=millis();
    }



    //is the communication in timeout?
    if (millis() > timeOutMillisAction+timeLastAction){
      //yes, delete the actual RFID read
      Serial.println("time out");
      actualUser[0]='\0';
      midAnalogRead=0;
      analogReadNumber=0;
    }
  }
}

String leggiRFID(){
  String ris = "*";  
  ris+=myID;
  ris+="#";

  char car;
  for (int i=0; i < rfidLength; i++){
    actualUser[i]=(char)mySerial.read();
    ris+=actualUser[i];
  }
  if (mySerial.read()!='\r'){
    Serial.println("Something went wrong 1");
    clearRubbish();
    return NULL;
  }
  if (mySerial.read()!='\n'){
    clearRubbish();
    Serial.println("Something went wrong 2");
    return NULL;
  }
  actualUser[rfidLength]=='\0';

  //Serial.println(ris); //must be println
  return ris;
}

void clearRubbish(){
  char lastC='0';
  while (mySerial.available()>0 && lastC!='\n'){
    lastC=mySerial.read();
  }
}

int minDiff = 10;
unsigned int lastReadAnag=0;

void writeForce(){
  if (analogReadNumber!=0){
    unsigned int newReadAnag = midAnalogRead/analogReadNumber;
    midAnalogRead=0;
    analogReadNumber=0;
    //there is difference from last read?
    //if ( abs(newReadAnag-lastReadAnag)>minDiff){
      //yes, print value to server
      lastReadAnag=newReadAnag;
      String ris = "*";  
      ris+=myID;
      ris+="~"; //tilde
      ris+=String(actualUser);
      ris+="+"; //tilde
      ris+=String(newReadAnag);
      Serial.println(ris); //must be println
      timeLastAction=millis();
    //}
  }
}



