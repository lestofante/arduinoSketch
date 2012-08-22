//data HIGH goes from 900 to 1700
//data LOW go to 19000

#define MINSERVO 50 //value to test
#define MAXSERVO 1024 //value to test
#define SERVOLOWSIGNALDURATION 2000

unsigned long duration1=MINSERVO;
unsigned long duration2=19000;

struct PPMPIN{
  byte pin;
  boolean pinstate = false;
  unsigned long lastChange;
  byte
}

struct PPMPIN test;

void setup()
{
  Serial.begin(9600);
  pinMode(PINOUTPUT, OUTPUT);
  test.pin=2;
  test.lastChange=micros();
}

void loop()
{
  updatePPM();
  digitalWrite(PINOUTPUT, HIGH);
  delayMicroseconds(duration1);
  digitalWrite(PINOUTPUT, LOW);
  delayMicroseconds(1000);
  
  duration1+=10;
  if (duration1>MAXSERVO){
    duration1=MINSERVO;
  }
  Serial.print("Moving to: ");
  Serial.println(duration1);
}

void updatePPM(){
  if (micros()-test.lastChange < SERVOLOWSIGNALDURATION){
    test.lastChange=micros();
    
  }
    
}

