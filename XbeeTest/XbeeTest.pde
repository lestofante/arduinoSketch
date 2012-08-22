void setup()  
{
  Serial.begin(9600);
  delay(2000);
  Serial.println("Goodnight moon!");
}
boolean stato = false;
void loop()                     // run over and over again
{
  if (Serial.read()!=-1){
    if (stato){
      digitalWrite(13, LOW);
      stato = false;
    }else{
      digitalWrite(13, HIGH);
      stato = true;
    }
  }
}
  /*
  if (Serial.available()) {
    if (stato){
      digitalWrite(13, LOW);
      stato = false;
    }else{
      digitalWrite(13, HIGH);
      stato = true;
    }
      
    Serial.print((char)Serial.read());
  }else{
    */
    /*
    Serial.print("test\n");
    delay(1000);
    if (stato){
      digitalWrite(13, LOW);
      stato = false;
    }else{
      digitalWrite(13, HIGH);
      stato = true;
    }
  //}
}*/
