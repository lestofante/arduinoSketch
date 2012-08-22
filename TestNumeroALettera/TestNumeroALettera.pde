void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Inserisci un numero e uno spazio o vai a capo:"); 
} 


byte cifre[10];
int cifreLette=0;
void loop() 
{
  if ( Serial.available() ){
    cifre[cifreLette]=Serial.read();
    if (cifre[cifreLette]==' ' || cifre[cifreLette]=='\n'){
      decifra();
      cifreLette=0;
    }else
      cifreLette++;
  }
}

void decifra (){
  int i;
  int numero=0;
  boolean negativo = false;
  if (cifre[0]=='-'){
    negativo=true;
    cifre[0]='0';
  }
  for (i=0; i < cifreLette;i++){
    if (cifre[i]<'0' || cifre[i] > '9'){
      Serial.print("valore sconosciuto e scaratato:");
      Serial.println(cifre[i]);
    }else{
      numero*=10;
      numero+= cifre[i]-'0';
    }
  }
  if (negativo){
    numero*=-1;
  }
  Serial.print("valore finale:");
  Serial.println(numero);
}
