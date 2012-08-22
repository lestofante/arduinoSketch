#define irEmitter 13            // polo positivo emettitore
#define irReceiverN 2          // polo negativo ricevitore
#define irReceiverP 5          // polo positivo ricevitore
volatile long distance0 = 0;
volatile long distance1 = 0;
volatile long distance2 = 0;
volatile long lightTime = 0;
volatile int currentInit = 0;
volatile long gain = 4; //gain crescita valore distanza
volatile long maxRangeValue = 666666; //valore range massimo
volatile long maxRange = 2000; //distanza range massimo in mm

void setup() {
  Serial.begin(57600);             // inizializza seriale a 9600 baud
  pinMode(irEmitter, OUTPUT);      // il led emettitore è un output
  pinMode(irReceiverP, OUTPUT);    // il polo positivo del let ricevitore è un output
  digitalWrite(irReceiverP, LOW);  // e deve essere LOW
}

//questa è un'ipotesi di scheduling (o qualcosa del genere)
void focus() { 
  if(currentInit == 0){attachInterrupt(0, one, LOW);}
  if(currentInit == 1){attachInterrupt(0, two, LOW);}
}

void one() {
  distance0 = micros() - lightTime;
  detachInterrupt(0);
  init(1);
}

void two(){
  distance1 = micros() - lightTime;
  distance2 = distance1 * (distance0 / (distance0 - distance1));
  distance2 = map(distance2, 0, maxRangeValue, 0, maxRange);
  if(distance2 <= 0) distance2 = 0;
  if(distance2 > 15) distance2 = sqrt(distance2) * gain;
  Serial.println(distance2); 
  distance1 = 0; 
  distance0 = 0;
  detachInterrupt(0);
  init(0);
}

void init(int emitter) {
  if(emitter == 0)digitalWrite(irEmitter, LOW);   
  if(emitter == 1)digitalWrite(irEmitter, HIGH); 
  lightTime = micros();
  pinMode(irReceiverN, OUTPUT);
  digitalWrite(irReceiverN, HIGH); //carico ricevitore di induttanza
  pinMode(irReceiverN, INPUT);
  digitalWrite(irReceiverN, LOW);
  if(emitter == 1) currentInit = 1;
  if(emitter == 0) currentInit = 0;
}

void loop() {
  focus(); 
}
