unsigned long tempo1=0;
unsigned int  scrollTime1=0;

unsigned int tempo2=0;
unsigned char  scrollTime2=0;

unsigned long tempo3=0;
unsigned char  scrollTime3=0;

unsigned long tempo4=0;
unsigned long  scrollTime4=0;

void setup () {
    delay(2000);
    Serial.begin(115200);
    Serial.println("Partito");
}
boolean ov=false;
unsigned long tempoOv2=0;
void loop () { 
    tempo1+=1;
    if (tempo1==0){
      Serial.print("O1");
      ov = true;
    }
    tempo2+=1;
    if (tempo2==0){
      Serial.print("O2");
      tempoOv2 = millis()-tempoOv2;
      Serial.print("t:");
      Serial.print(tempoOv2); 
      Serial.print(" ");     
      Serial.print(tempo3, DEC);
      
      //ov = true;
    }
    tempo3+=1;
    if (tempo3==0){
      Serial.print("O3");
      ov = true;
    }
    tempo4+=1;
    if (tempo4==0){
      Serial.print("O4");
      ov = true;
    }
    if (ov){
      Serial.println();
      ov=false;
    }

    if (tempo1 >= scrollTime1 + 20) {
      Serial.print("1 ");
      Serial.println(scrollTime1, DEC);
        if (tempo1 - scrollTime1 != 20){
          Serial.print("errore1 ");
          Serial.print(tempo1, DEC);
          Serial.print(" ");
          Serial.println(scrollTime1, DEC);
          Serial.println((tempo1 - scrollTime1), DEC);
        }
        scrollTime1 = tempo1;
    }
      if (4294967000L > tempo1 && tempo1 > 70000L){ //don't wait too much
        Serial.print("fast forward1");
        tempo1 = 4294967000L;
      }
    
    
/*
    if (tempo2 >= scrollTime2 + 20 && tempo2 < 300) {
      Serial.print("2 ");
      Serial.print(tempo2, DEC);
          Serial.print(" ");
      Serial.println(scrollTime2, DEC);
        if (tempo2 - scrollTime2 != 20){
          Serial.print("errore2 ");
          Serial.print(tempo2, DEC);
          Serial.print(" ");
          Serial.print(tempo2 - scrollTime2, DEC);
          Serial.print(" ");
          Serial.println(scrollTime2, DEC);
        }
        scrollTime2 = tempo2;
    } //fast forward con int non necessario, l'overflow avviene in un secondo circa a 16MHz

    if (tempo3 >= scrollTime3 + 20 && tempo3 < 300) {
      Serial.print("3 ");
      Serial.println(scrollTime3, DEC);
        if (tempo3 - scrollTime3 != 20){
          Serial.print("errore3 ");
          Serial.print(tempo3, DEC);
          Serial.print(" ");
          Serial.println(scrollTime3, DEC);
        }
        scrollTime3 = tempo3;
    }
      if (4294967000L > tempo3 && tempo3 > 300){ //don't wait too much
        Serial.print("fast forward3");
        tempo3 = 4294967000L;
      }
    
  /*  
    if (tempo4 >= scrollTime4 + 20) {
      Serial.print("4 ");
      Serial.println(scrollTime4, DEC);
        if (tempo4 - scrollTime4 != 20){
          Serial.print("errore4 ");
          Serial.print(tempo4, DEC);
          Serial.print(" ");
          Serial.println(scrollTime4, DEC);
        }
        scrollTime4 = tempo4;
    }else{
      if (4294967000L > tempo4){ //don't wait too much
        Serial.print("fast forward4");
        tempo4 = 4294967000L;
      }
    }
    */
}
