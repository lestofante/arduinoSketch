unsigned long Tempo;
unsigned long  ScrollTime;
const byte LED=13;
boolean stato=true;
unsigned long secondi;
int i = 100000;

void setup () {
    delay(2000);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, stato);
    Serial.begin(19200);
    Serial.println("Partito");
}

void loop () { 
    Tempo+=1;

    if (Tempo >= ScrollTime + 300) {
    //  Serial.print('S');
    //    Serial.println(ScrollTime, DEC);
    //    Serial.println(Tempo, DEC);
        stato ^= true;
        digitalWrite(LED, stato);
    //  Serial.print('T');
    //    Serial.println((Tempo - (ScrollTime + 300)), DEC);
        if (Tempo - ScrollTime < 300){
          Serial.println("azz");
        }
        ScrollTime = Tempo;
    } 
}
