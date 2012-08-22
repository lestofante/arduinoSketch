#include "pins_arduino.h"
uint8_t PCintLast[3];

ISR(PCINT0_vect) {
  int port=0;
  
  // get the pin states for the indicated port.
  uint8_t curr;
  uint8_t mask;
  
  curr = *portInputRegister(port+2);
  mask = curr ^ PCintLast[port]; //evita che lo stesso pin venga estratto due o più volte se più interrupt si sovreppongono
  PCintLast[port] = curr;
  Serial.print("curr");
  Serial.println(curr, BIN);

  Serial.print("PINB");
  Serial.println(PINB, BIN);
  
  Serial.print("mask");
  Serial.println(mask, BIN);

  if ((mask &= PCMSK0) == 0) {
    //ritorna se l'iterrupt è lanciato da un pin già estratto o non presente in PCMSK0 (quindi un pin con interrupt non attivato)
    return;
  }
  
  uint8_t bitt;
  
  for (uint8_t i=0; i < 8; i++) {//per 8 volte
    bitt = 0x01 << i;//crea il valore 2^i
    if (bitt & mask) { //usato per estrarre l'iesimo bit
      uint8_t pin = port * 8 + i-1; //calcola il valore PICINT del pin
      Serial.print("Interrupt di PCINT:");
      Serial.println(pin, DEC);
    }
  }
}

ISR(PCINT1_vect) {
  //come codice ISR0 ma con port = 1
}
ISR(PCINT2_vect) {
  //come codice ISR0 ma con port = 2
}

void setup(){
  Serial.begin(9600);

/*I SEGUENTI SETTAGGI SONO MODIFICABILI A SECONDA DELLE NECESSITÀ , MAGGIORI INFO SUL MANUALE DELL'ATMEGA*/
  
  Serial.println("\nPCICR:");
  Serial.println(PCICR, BIN);
  // su quale serie di pin voglio abilitare gli interrupt?
  PCICR |= (1 << PCIE0); // PCINT0-5 DigitalPIN8-13
  //PCICR |= (1 << PCIE1); // PCINT8..13 AnalogPIN0-5
  PCICR |= (1 << PCIE2); // PCINT16..23 DigitalPIN0-7 ATTENZIONE!!! RILEVA COME INTERRUPT ANCHE LA COMUNICAZIONE SERIALE
  Serial.println(PCICR, BIN);
  
  /*ATTENZIONE, DI SEGUITO SI SETTANO GLI INTERRUPT SUI PIN. GESTIRE SEMPRE TUTTI QUELLI DICHIARATI CON L'APPOSITA FUNZIONE ISR(PCINTXX_VECT) o SIGNAL(PCINTXX_VECT) DOVE xx È IL NUMERO DEL PCMSK CORRISPONDENTE */

 
  Serial.println("PCMSK0:");  
  Serial.println(PCMSK0, BIN);
  //abilitiamo gli interrupt pin per digitalpin da 8 a 14, in questo caso solo 8 corrispondente a PCINT0 e 9 -> PCINT1
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1);
  Serial.println(PCMSK0, BIN);
/*
  Serial.println("PCMSK1:");  
  Serial.println(PCMSK1, BIN);  
  //abilitiamo gli interrupt pin per analogpin da 0 a 5, in questo caso tutti
  PCMSK1 |= B01111111; //PCMSK1 È L'UNICO REGISTRO PCMSK DI CUI SI PUÒ USARE AL MASSIMO SOLO 7 BIT
  Serial.println(PCMSK1, BIN);
*/
/*
  Serial.println("PCMSK2:");  
  Serial.println(PCMSK2, BIN);
  //abilitiamo gli interrupt pin per digitalpin da 16 a 23, in questo caso i PCINT 18,20,21,22,23 che corrispondono ai pin digitali 2,4,5,6,7
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) | (1 << PCINT23);
  Serial.println(PCMSK2, BIN);
*/
 /*FINE INTERRUPT SUI PIN*/
 pinMode(9, INPUT);
// pinMode(, INPUT); 
}

void loop(){
  Serial.println("loop");
  //Serial.println(changed);
}
