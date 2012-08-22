#include "swRTC.h"
swRTC rtc;

void setup() {
    delay(2000);
    Serial.begin(9600);
    if (!rtc.setRTC(22,10,0,25,9,2011)) {
        Serial.println("Orologio non inizializzato");
    } else {
        Serial.println("Orologio inizializzato");
    }
}

void loop() {
    Serial.print(rtc.getHours(), DEC);
    Serial.print(":");
    Serial.print(rtc.getMinutes(), DEC);
    Serial.print(":");
    Serial.println(rtc.getSeconds(), DEC);
    delay(1000);
}


