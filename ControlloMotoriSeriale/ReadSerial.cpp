#include "ReadSerial.h"


ReadSerial::ReadSerial(){
  indexInput2=0;
  inputValue=false;
  validInput=false;
}

bool ReadSerial::readSerial(){
  /*
  input data: *name:value\n
  */
  while (Serial.available() > 0){
    char c = Serial.read();
    if (c =='*'){
      indexInput2=0;
      inputValue=false;
      validInput=true;
      Serial.println("starting input");
    }else{
      
      if (validInput){
        if (c =='\n'){
          input2[indexInput2]='\0';
          Serial.print("value ok");
          Serial.println(input2);
          int point=1, i=0;
          float ris=0;
          while(input2[i]!='\0'){
            if (input2[i]=='.'){
              point=indexInput2-i;
            }else{
              ris=(ris*10)+input2[i]-'0';
            }
            i++;
          }
          Serial.print("ris and point ");
          Serial.print(ris);
          Serial.print(" ");
          Serial.println(point);
          ris/=point;
          finalC=input1;
          finalF=ris;
          validInput=false;
          indexInput2=0;
          inputValue=false;
          return true;
        }else{
          if (inputValue){
            input2[indexInput2]=c;
            indexInput2++;
          }else{
            if (c ==':'){
              Serial.println("command ok, waiting value");
              inputValue=true;
            }else{
              Serial.print("command ");
              Serial.println(c);
              input1=c;
            }
          }
        }
      }
      
    }
  }
  return false;
}

void ReadSerial::get(char *risC, float *risF){
  (*risC)=finalC;
  (*risF)=finalF;
}
