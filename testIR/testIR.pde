int ricevitore = 2;
int emettitore = 9;

void setup(){
  Serial.begin(115200);
  pinMode(ricevitore, INPUT);
  pinMode(emettitore, OUTPUT);
}

long ris;
void loop(){
  digitalWrite(emettitore, LOW);
  ris = contaFallingBlocking(1000);
  Serial.print("numero falling LOW:");
  Serial.println(ris);
  digitalWrite(emettitore, HIGH);
  ris = contaFallingBlocking(1000);
  Serial.print("numero falling HIGH:");
  Serial.println(ris);
}

volatile long countFalling;
long contaFallingBlocking(unsigned long time){
  digitalWrite(ricevitore, HIGH);
  digitalWrite(ricevitore, LOW);
  countFalling=0;
  attachInterrupt(0, contaFalling, FALLING); //0 means digitalInput 2
  delay(time);
  detachInterrupt(0);
  return countFalling;
}

void contaFalling(){
  countFalling++;
}
