// Include the Wire library so we can start using I2C.
#include <Wire.h>
// Include the Love Electronics ADXL345 library so we can use the accelerometer.
#include "ADXL345.h"

// Declare a global instance of the accelerometer.
ADXL345 accel;

// Set up a pin we are going to use to indicate our status using an LED.
int statusPin = 2; // I'm using digital pin 2.

void setup()
{
  // Begin by setting up the Serial Port so we can output our results.
  Serial.begin(9600);
  // Start the I2C Wire library so we can use I2C to talk to the accelerometer.
  Wire.begin();

  // Ready an LED to indicate our status.
  pinMode(statusPin, OUTPUT);

  // Create an instance of the accelerometer on the default address (0x1D)
  accel = ADXL345();

  // Check that the accelerometer is infact connected.
  if(accel.EnsureConnected())
  {
    Serial.println("Connected to ADXL345.");
    digitalWrite(statusPin, HIGH); // If we are connected, light our status LED.
  }
  else 
  {
    Serial.println("Could not connect to ADXL345.");
    digitalWrite(statusPin, LOW); // If we are not connected, turn our LED off.
  }
}

void loop() {
}

