/*
  Single_Temp.pde - Example using the MAX6675 Library.
 Created by Ryan McLaughlin <ryanjmclaughlin@gmail.com>
 */

#include "MAX6675.h"

int LED1 = 9;             // Status LED Pin
int CS = 10;              // CS pin on MAX6675
int SO = 12;              // SO pin of MAX6675
int SCKasd = 13;             // SCK pin of MAX6675
int units = 1;            // Units to readout temp (0 = ˚F, 1 = ˚C, others = raw)
float error = 0.0;        // Temperature compensation error
float temperature = 0.0;  // Temperature output variable


// Initialize the MAX6675 Library for our chip
MAX6675 temp(CS,SO,SCK,units);


// Setup Serial output and LED Pin
// MAX6675 Library already sets pin modes for MAX6675 chip!
void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
}

void loop() {
  // Read the temp from the MAX6675
  temperature = temp.read_temp();

  if(temperature < 0) {
    // If there is an error with the TC, temperature will be < 0
    Serial.print("Thermocouple Error on CS");
    Serial.println( temperature );
    digitalWrite(LED1, HIGH);
  } 
  else {
    Serial.print("Current Temperature: ");
    Serial.print( temperature );
    digitalWrite(LED1, LOW);
  }
  
  temperature = myMap(temperature/0.25, 0, 4095, -270, 1372);
  Serial.print(" : ");
  Serial.print( temperature );
  
  temperature = myMap2(temperature/0.25, 0, 4095, -270, 1372);
  Serial.print(" : ");
  Serial.print( temperature );
  
  temperature = myMap(temperature/0.25, 0, 4095, 0, 1372);
  Serial.print(" : ");
  Serial.print( temperature );
  
  temperature = myMap2(temperature/0.25, 0, 4095, 0, 1372);
  Serial.print(" : ");
  Serial.println( temperature );
  // Wait one second before reading again
  delay(500);
}

long myMap(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long myMap2(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min);
}
