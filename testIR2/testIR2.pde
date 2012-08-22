int ricevitore = 2;
int emettitore = 9;

void setup(){
  Serial.begin(115200);
  pinMode(ricevitore, INPUT);
  pinMode(emettitore, OUTPUT);
  delay(1000);
}

unsigned long ris, lastAlto=0;
boolean statoEmettitore=true;
void loop(){
  //accende e spegne il led emettitore, e ne stampa lo stato
  if (statoEmettitore){
    statoEmettitore=false;
    Serial.print("f ");
  }
  else{
    statoEmettitore=true;
    Serial.print("t ");
  }
  digitalWrite(emettitore, statoEmettitore);

  //legge la durata del segnale HIGH sul ricevitore e la stampa a video
  ris=durataHigh();
  Serial.println( ris );
  
  if (statoEmettitore){
    lastAlto=ris;
  }else{
    if (lastAlto<ris){
      Serial.print( "distanza: " );
      Serial.println( ris-lastAlto );
    }else{
      Serial.println( "lettura incongruente" );
    }
  }
}


//FUNZIONE DI LETTURA BLOCCANTE
//per implementare un time-out basta modificare il while
//non è gestito l'overflow di micro()
unsigned long timeHigh;
volatile unsigned long timeLow;

unsigned long durataHigh(){
  timeHigh=micros();
  timeLow=0;//inizializza la lettura a un valore noto, che fungerà da "tappo" per il while
  digitalWrite(ricevitore, HIGH);//attiva pullUP, per caricare il ricevitore
  attachInterrupt(0, tempoLow, FALLING);//vogliamo intercettare quando il digitalPin2 diverrà LOW ("cade" il segnale da 1 a 0)
  digitalWrite(ricevitore, LOW);//disattiva pullUP, il ricevitore è carico

  //attende che la lettura sia completata
  while ( timeLow==0 )//finchè c'è il valore di tappo (il ricevitore è ancora HIGH)
    ;//non fare niente

  detachInterrupt(0);
  return timeLow-timeHigh;
}

//FUNZIONE DI INTERRUPT
//più minimale di così non mi viene :-)
void tempoLow(){
  timeLow=micros();
}

