#include <Servo.h>
//#include <NewSoftSerial.h>

#include "InputPin.h"
#include "Stabilizzazione.h"
#include "Utils.h"
#include "EngineMindPID.h"


#define ledPin 13

#define DEBUG_TIME 1000000L //1 second
#define DEBUG_ORIZON_TIME_MICROS 20000L //50 fps

#define ROLL_STABLE_ANGLE 0
#define PITCH_STABLE_ANGLE 0
#define YAW_STABLE_ANGLE 0

Stabilizzazione orizzonte;
EngineMind engines;

//inpit from RX
InputPin rxInputs;

//NewSoftSerial serial(7, 8);

unsigned long timeLastLoop;
void setup(){
  delay(5000);
  Serial.begin(19200);
  //serial.begin(19200);
  
  Serial.println("INITILIZING ENGINE");
  engines.start();

  Serial.println("INITILIZING ORIZ");  
  orizzonte.start();
  /*  
   Serial.println("INITILIZING WATCHDOG");  
   TIMSK2 |= (1<<TOIE2);
   */

  delay(5000);
  Serial.println("DONE SETUP");
  
  timeLastLoop=micros();
}

volatile int loopOk=0;
volatile boolean error = false;
void loop(){
  loopOk++;
  if (!error){
    //Serial.println("loop");
    loopQuadri();
    digitalWrite(ledPin, LOW);
  }
  else{
    Serial.println("ERROR");
    digitalWrite(ledPin, HIGH);
  }
}

int loopNumber=0;
unsigned long timeLastDebug = 0;
unsigned long timeLastDebugOrizon = 0;
unsigned long actualTimeMicros = 0;
//unsigned long timeLastLoop = micros();
unsigned long sumLoopTime = 0;
unsigned long durationLoop;

unsigned long time;
unsigned long sumPart0=0;
unsigned long sumPart1=0;
unsigned long sumPart2=0;
unsigned long sumPart3=0;
unsigned long sumPart4=0;
unsigned long sumPart5=0;

void loopQuadri(){
  actualTimeMicros = time = micros();
  
  durationLoop = actualTimeMicros-timeLastLoop;
  timeLastLoop=actualTimeMicros;
  
  sumLoopTime+=durationLoop;

  loopNumber++;
  //Serial.println("up orizzonte");
  orizzonte.update(durationLoop);
  //Serial.println("fine up orizzonte");
  
  sumPart0+=micros() - time;
  time=micros();

  /*
  float roll=atan2(2*(orizzonte.q0*orizzonte.q1+orizzonte.q2*orizzonte.q3), 1-2*(orizzonte.q1*orizzonte.q1+orizzonte.q2*orizzonte.q2));
   float pitch=asin( 2*(orizzonte.q0*orizzonte.q2-orizzonte.q1*orizzonte.q3) );
   float yaw=atan2(2*(orizzonte.q0*orizzonte.q3+orizzonte.q1*orizzonte.q2), 1-2*(orizzonte.q2*orizzonte.q2+orizzonte.q3*orizzonte.q3));
   */
  float actualAngle[3];
  fromQuaternionToEuler(actualAngle);
  float roll = actualAngle[1];
  float yaw = actualAngle[0];
  float pitch = actualAngle[2];
  sumPart1+=micros() - time;
  time=micros();
  
/*
  actualAngle[0] = roll = util.normalize(roll);
  actualAngle[1] = pitch = util.normalize(pitch);
  actualAngle[2] = yaw = util.normalize(yaw);
*/
  /*
  //USER INPUT
   //angle in radiant!!
   */
  int enginesPower=0;
  float desiredAngle[3];
  enginesPower=rxInputs.getDuration(0, 0, 100);
  

  //
  // FAKE INPUT
  //
  desiredAngle[0] = roll = -0+ROLL_STABLE_ANGLE;
  desiredAngle[1] = pitch = 0+PITCH_STABLE_ANGLE;
  desiredAngle[2] = yaw = 0+YAW_STABLE_ANGLE;
  //
  // END FAKE INPUT
  //

  /*
  //
   // REAL INPUT
   //
   desiredAngle[0] = pitch = rxInputs.getAngle(1)+PITCH_STABLE_ANGLE;
   desiredAngle[1] = roll = -rxInputs.getAngle(2)+ROLL_STABLE_ANGLE;
   desiredAngle[2] = yaw = rxInputs.getAngle(3)+YAW_STABLE_ANGLE;
   //
   // END REAL INPUT
   //
   */

  /*
  //END USER INPUT
   */

  sumPart2+=micros() - time;
  time=micros();
  
  //UPDATE ENGINEMIND
  if ( loopNumber % 20 == 0){ //how much fast it has to run, 100=200millis it time for loop = 2000micros
    //engines.update(actualAngle, desiredAngle, enginesPower);
    
    /*
      FOR DEBUG
    */
    engines.update(actualAngle, desiredAngle, 0);
    /*
      END DEBUG
    */
  }

  sumPart3+=micros() - time;
  time=micros();

  if (actualTimeMicros-timeLastDebugOrizon>DEBUG_ORIZON_TIME_MICROS){
  //if (loopNumber > DEBUG_TIME){
    // print the orizon quaternion
    Serial.print(orizzonte.q0);
    Serial.print(" ");
    Serial.print(orizzonte.q1);
    Serial.print(" ");
    Serial.print(orizzonte.q2);
    Serial.print(" ");
    Serial.print(orizzonte.q3);
    Serial.println();
/*    
    serial.print(orizzonte.q0);
    serial.print(" ");
    serial.print(orizzonte.q1);
    serial.print(" ");
    serial.print(orizzonte.q2);
    serial.print(" ");
    serial.print(orizzonte.q3);
    serial.println();
*/    
    timeLastDebugOrizon=micros();
  }

  sumPart4+=micros() - time;
  time=micros();

  if (actualTimeMicros-timeLastDebug > DEBUG_TIME){
    Serial.print("Engine power: ");
    Serial.println(enginesPower);
    engines.debug();
    
    Serial.print("Micros loop:");
    
    sumLoopTime/=loopNumber;
    Serial.println(sumLoopTime);
    
    orizzonte.setTime(sumLoopTime);
    sumLoopTime=0;

    Serial.println("Agoli attuali");
    fromQuaternionToEuler(actualAngle);
    for (int i=0; i < 3; i++){
      Serial.print(actualAngle[i]);
      Serial.print(" ");
    }
    Serial.println();
    readSerial();
    
    orizzonte.debug();

    Serial.print("time0:");
    Serial.println(sumPart0/loopNumber);    
    Serial.print("time1:");
    Serial.println(sumPart1/loopNumber);
    Serial.print("time2:");
    Serial.println(sumPart2/loopNumber);
    Serial.print("time3:");
    Serial.println(sumPart3/loopNumber);
    Serial.print("time4:");
    Serial.println(sumPart4/loopNumber);
    Serial.print("time5:");
    Serial.println(sumPart5/loopNumber);
    
    loopNumber=sumPart0=sumPart1=sumPart2=sumPart3=sumPart4=sumPart5=0;
    timeLastDebug=actualTimeMicros;
    Serial.println();
  }
  
  sumPart5+=micros() - time;

}

char input1;
char input2[10];
int indexInput2=0;
boolean inputValue=false;
boolean negativo=false;
void readSerial(){
  /*
  input data: *name:value\n
   */
  while (Serial.available() > 0){
    char c = Serial.read();
    if (c =='*'){
      indexInput2=0;
      inputValue=false;
      negativo=false;
      //      Serial.println("starting input");
    }
    else{
      if (c =='\n'){
        input2[indexInput2]='\0';
        //        Serial.print("value: ");
        //        Serial.println(input2);
        int point=0, i=0;
        float ris=0;
        while(input2[i]!='\0'){
          if (input2[i]=='.'){
            point=indexInput2-i-1;
          }
          else{
            ris=(ris*10)+input2[i]-'0';
          }
          i++;
        }
        /*      
         Serial.print("ris: ");
         Serial.println(ris);
         Serial.print("point: ");
         Serial.println(point);
         */
        ris/=pow(10, point);
        if (negativo){
          ris*=-1;
        }
        executeCommand(input1, ris);
      }
      else{
        if (inputValue){
          if (indexInput2==0 && c=='-'){
            negativo = true;
          }
          else{
            input2[indexInput2]=c;
            indexInput2++;
          }
        }
        else{
          if (c ==':'){
            //            Serial.println("command ok, waiting value");
            inputValue=true;
          }
          else{
            input1=c;
          }
        }
      }
    }
  }

}

void executeCommand(char c, float v){
  Serial.print("setting command:value");
  Serial.print(c);
  Serial.println(v);

  int i;

  switch(c){
    //engine
    case ('p'):
    for (i=0; i < 4; i++){
      //pid[i].setP( v );
      engines.setP( v );
    }
    break;
    case ('i'):
    for (i=0; i < 4; i++){
      //pid[i].setI( v );
      engines.setI( v );
    }
    break;
    case ('d'):
    for (i=0; i < 4; i++){
      //pid[i].setD( v );
      engines.setD( v );
    }
    break;
    //stabilization
    case ('P'):
    orizzonte.setKp(v);
    break;
    case ('I'):
    orizzonte.setKi(v);
    break;
  }
}

void fromQuaternionToEuler(float *euler){
  //        q1.normalize  ();
  float test = orizzonte.x * orizzonte.y + orizzonte.z * orizzonte.w;
  if (test > 0.499) { // singularity at north pole
    euler[1] = (float) (2 * atan2(orizzonte.x, orizzonte.w));
    euler[2] = (float) (PI / 2);
    euler[0] = 0;
    return;
  }
  if (test < -0.499) { // singularity at south pole
    euler[1] = (float) (2 * atan2(orizzonte.x, orizzonte.w));
    euler[2] = (float) -(PI / 2.0);
    euler[0] = 0;
    return;
  }
  float sqx = orizzonte.x * orizzonte.x;
  float sqy = orizzonte.y * orizzonte.y;
  float sqz = orizzonte.z * orizzonte.z;
  euler[1] = (float) atan2(2 * orizzonte.y * orizzonte.w - 2 * orizzonte.x * orizzonte.z, 1 - 2 * sqy - 2 * sqz);
  euler[2] = (float) asin(2 * test);
  euler[0] = (float) atan2(2 * orizzonte.x * orizzonte.w - 2 * orizzonte.y * orizzonte.z, 1 - 2 * sqx - 2 * sqz);
  return;
}

/*
 * WATCHDOG!!!!
 *
volatile unsigned long timeLock = micros();
ISR(TIMER2_OVF_vect) {
  if (micros() > timeLock){
    if (micros()-timeLock >= 100000){//10 times at second
      timeLock=micros();
      //Serial.println(time);
      if (loopOk!=0 && rxInputs.getAndSetFalseHasChanged()){//if loop is running && we are getting signal from receiver
        loopOk=0;
        error=false;
      }
      else{
        error=true;
        //loop is locked OR tx is lost!!! TURN DOWN ENGINE!
        engines.brake();
        //  digitalWrite(ledPin, HIGH);
      }
    }
  }
};
*/



