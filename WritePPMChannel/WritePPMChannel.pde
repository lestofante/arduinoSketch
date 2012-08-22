//data HIGH goes from 900 to 1700
//data LOW go to 19000

#define MINSERVO 900
#define MAXSERVO 2000
#define PINOUTPUT 2
unsigned long duration1=MINSERVO;
unsigned long duration2=1000;//ok da 100, ma non fa in tempo a eseguire i comandi
unsigned long durationSerial=1000;

void setup()
{
  Serial.begin(9600);
  pinMode(PINOUTPUT, OUTPUT);
  Serial.print("Delay LOW: ");
  Serial.println(duration2);
}

byte in;
void loop()
{
  digitalWrite(PINOUTPUT, HIGH);
  delayMicroseconds(duration1);
  digitalWrite(PINOUTPUT, LOW);
  delayMicroseconds(duration2);
  
  if (Serial.available() > 0) {
    in = Serial.read();
    if (in == '+'){
      duration1+=100;
      if (duration1>MAXSERVO){
        duration1=MINSERVO;
      }
    }else{
      duration1-=100;
      if (duration1<MINSERVO){
        duration1=MINSERVO;
      }
    }
    Serial.print("New duration: ");
    Serial.println(duration1);
    
  }
  if (duration1%100==0){
    Serial.print("Moving to: ");
    Serial.println(duration1);
  }
}
