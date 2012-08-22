//data UP goes from 900 to 1700
//data DOWN go to 19000

int pin = 2;
unsigned long duration1;
unsigned long duration2;
unsigned long time;
void setup()
{
  Serial.begin(115200);
  //pinMode(pin, INPUT);
  //Serial.println(DDRD, BIN);
  //DDRD = DDRD | B00000000;
  //Serial.println(DDRD, BIN);
  time = micros();
}

void loop()
{
  //Serial.println(PORTD, BIN);
  //Serial.println(PIND, BIN);
  //Serial.println();
  //duration1 = pulseIn(pin, HIGH);
  //duration2 = pulseIn(pin, LOW);
  analogWrite(3, 200);
  time=micros()-time;
  Serial.print("Time");
  Serial.println(time);
  Serial.print("high:");
  Serial.println(duration1);
  Serial.print("low:");
  Serial.println(duration2);
  time=micros();
}

