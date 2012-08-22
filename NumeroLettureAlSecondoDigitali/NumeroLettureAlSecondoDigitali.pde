const int ledPin =  13;      // the number of the LED pin


long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, INPUT);      
}
int io;
long readNumber=0;
void loop()
{
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    Serial.print("Letture al secondo:");
    Serial.println(readNumber);
    readNumber=0;
    previousMillis = millis();
  }
  io=digitalRead(ledPin);
  readNumber++;
}

