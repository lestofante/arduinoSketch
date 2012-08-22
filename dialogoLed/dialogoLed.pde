#include <layer1.h>

Layer1 com;

void setup() {
  Serial.begin(115200);
  com.begin(A0, false, 1000);
  
  /* Tutte e 3 le begin commentate che seguino sono valide! */
  //com.begin(A0, false);
  //com.begin(A0);
  //com.begin();  
}


void loop() {
    if (com.isReceiver()) {
      byte r = com.startRX();
      if (r != 0) Serial.print(r, BYTE);
    } else {
        char test[] = "Bi-directional LED com by Gioblu.com\n";
        for (int i = 0; test[i] != '\0'; i++)
        com.byteTX(test[i]);
      }
  }
