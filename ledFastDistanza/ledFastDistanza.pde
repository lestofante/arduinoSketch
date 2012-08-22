int inputPIN = A0;
int emitterPIN = A1;
int intensity = 0;
int maxValue = 0;
int distance = 0;
int readings =10;

void setup() {
pinMode(inputPIN, INPUT);
pinMode(emitterPIN, OUTPUT);
pinMode(A2, OUTPUT);
digitalWrite(A2, LOW);
Serial.begin(9600);
delay (2000);
}

int getIntensity(boolean emitter) {
 intensity = 0;
 //Serial.println(emitter ? "HIGH " : "LOW ");
 digitalWrite(emitterPIN, emitter ? HIGH : LOW);
 int r;
 for(int i = 0; i < readings; i++) {  
   r=analogRead(inputPIN);
   intensity = intensity + r;
   //Serial.println(r);
 }
 //Serial.print("mid:");
 //Serial.println(intensity/readings);  
 delay(100);
 return intensity;
}

void getDistance() {
  intensity = getIntensity(true) - getIntensity(false);
  //if(maxValue < intensity) 
  //  maxValue = intensity;
  //intensity = map(intensity, maxValue, 0, 0, 2000);
}
 

void loop() {
  //if (Serial.read() != -1){
    getDistance();
   // Serial.print(maxValue);
   // Serial.print(" ");
    Serial.println(intensity);  
  //}
}
