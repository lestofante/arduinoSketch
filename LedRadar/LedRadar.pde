#define irEmitter 13            // polo positivo emettitore
#define irReceiverN 2          // polo negativo ricevitore
#define irReceiverP 5          // polo positivo ricevitore
volatile unsigned long distance0 = 0;
volatile unsigned long distance1 = 0;
volatile unsigned long distance2 = 0;
volatile unsigned long lightTime = 0;
volatile boolean nuovaLettura = false;
volatile unsigned long lostRead = 0;

unsigned long lastRead = 0;

void setup() {
  Serial.begin(9600);             // inizializza seriale a 9600 baud
  pinMode(irEmitter, OUTPUT);      // il led emettitore è un output
  pinMode(irReceiverP, OUTPUT);    // il polo positivo del let ricevitore è un output
  digitalWrite(irReceiverP, LOW);  // e deve essere LOW
  
  //start led read
  init(false);
  attachInterrupt(0, one, FALLING);
  lastRead=millis();
}

void one() {
  detachInterrupt(0);
  distance0 = micros() - lightTime;
  init(true);
  attachInterrupt(0, two, FALLING);
}

void two(){
 detachInterrupt(0);
 distance1 = micros() - lightTime;
 distance2 = distance0 - distance1;
 distance1 = 0; 
 distance0 = 0;
 if (nuovaLettura){
   lostRead++;
 }else{
   nuovaLettura=true;
   lostRead=0;
 }
 
 init(false);
 attachInterrupt(0, one, FALLING);
}

void init(boolean emitterOn) {
 if(emitterOn)
   digitalWrite(irEmitter, HIGH);
 else
   digitalWrite(irEmitter, LOW);
   
 lightTime = micros();
 pinMode(irReceiverN, OUTPUT);
 digitalWrite(irReceiverN, HIGH); //carico ricevitore di induttanza
 pinMode(irReceiverN, INPUT);
 digitalWrite(irReceiverN, LOW);
}

int tempLostRead;
unsigned long time_out_millis = 1000;//1 secondo
void loop() {
  if (nuovaLettura){
    nuovaLettura=false;
    tempLostRead=lostRead;//questo perchè le serial sono lente, e se non si azzera lostRead subito potremmo avere una lettura inconsistente dopo
    Serial.print("D: "); //disatnza
    Serial.println(distance2);
    Serial.print("L: "); //letture perse
    Serial.println(tempLostRead);
    Serial.println(); //lascia una riga vuota
    
    lastRead=millis();
  }
  
  if (millis()-time_out_millis > lastRead){
    Serial.println("ERROR! Time Out or millis() Overflow! restarting interrupt:");
    detachInterrupt(0);
    init(false);
    attachInterrupt(0, one, FALLING);
    lastRead=millis();
  }
}
