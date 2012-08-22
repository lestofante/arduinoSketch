int inputPIN = A0;
int emitterPIN = A1;
int minDistanceIntensity = 0;
double intensity = 1;
double distance = 1;
double ambient = 1;
int readings = 200;
int filterValue = 0.8;

void setup() {
  pinMode(inputPIN, INPUT);
  pinMode(emitterPIN, OUTPUT);
  pinMode(A2, OUTPUT);
  Serial.begin(115200);
  delay(2000);
}

double getDistance() {
  ambient = 0;
  intensity = 0;
  digitalWrite(emitterPIN, LOW); // Lettura ambientale
  for(int i = 0; i < readings; i++) {
    ambient = ambient + analogRead(inputPIN);
  }
  digitalWrite(emitterPIN, HIGH); // Lettura attiva
  for (int i = 0; i < readings; i++) {
    intensity = intensity + analogRead(inputPIN);
  } 
  intensity = intensity - ambient; // Filtro luce ambientale
  distance=0;
  if(intensity >= 0){
    if(intensity > minDistanceIntensity) 
      minDistanceIntensity = intensity; // Salva l'intensità maggiore percepita
    distance = (distance * filterValue) + ((minDistanceIntensity / intensity) * (1 - filterValue)) * 1000;
  }
  return distance;
}

void loop() {
  double distance = getDistance();
  distance = abs(distance);

  Serial.println(distance);
  //Serial.println( analogRead(inputPIN) );
  //Serial.println();
}

