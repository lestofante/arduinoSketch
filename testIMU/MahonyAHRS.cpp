#include "MahonyAHRS.h"

/// <summary>
/// Initializes a new instance of the <see cref="MadgwickAHRS"/> class.
/// </summary>
/// <param name="samplePeriod">
/// Sample period.
/// </param>
/// <param name="kp">
/// Algorithm proportional gain.
/// </param>
/// <param name="ki">
/// Algorithm integral gain.
/// </param>
void MahonyAHRS::begin(float kp, float ki)
{
  //SamplePeriod = samplePeriod;
  Kp = kp;
  Ki = ki;
  Quaternion[0] = 1;
    Quaternion[1] = 0;
      Quaternion[2] = 0;
        Quaternion[3] = 0;
        
  eInt[0] = eInt[1] = eInt[2] = 0;
  
  lastUp = micros();
}

void MahonyAHRS::update(struct data sensor) {
  float gx=sensor.giroX, gy=sensor.giroY, gz=sensor.giroZ,
        ax=sensor.accX,   ay=sensor.accY,   az=sensor.accZ,
        mx=sensor.magneX, my=sensor.magneY, mz=sensor.magneZ;

  if (gx != -1 && gy!= -1 && gz != -1 && !floatEqZero(ax) && !floatEqZero(ay) && !floatEqZero(az) ){
    now = micros();
    /*
    Serial.print("UPPO:");
    Serial.print( (now-lastUp)/1000000.0 );
    Serial.print(":");
    Serial.print(gx,10);
    Serial.print(":");
    Serial.print(gy,10);
    Serial.print(":");
    Serial.print(gz,10);
    Serial.print("____");
    Serial.print(ax,10);
    Serial.print(":");
    Serial.print(ay,10);
    Serial.print(":");
    Serial.print(az,10);
    Serial.print("____");
    Serial.print(mx,10);
    Serial.print(":");
    Serial.print(my,10);
    Serial.print(":");
    Serial.print(mz,10);
    Serial.println(); 
    */
    if ( !floatEqZero(mx) && !floatEqZero(my) && !floatEqZero(mz) ){
      /*
      Serial.println("con magne!");
      Serial.print(sensor.magneX,10);
      Serial.print(":");
      Serial.print(sensor.magneY,10);
      Serial.print(":");
      Serial.print(sensor.magneZ,10);
      Serial.println();
      */
      Update(gx, gy, gz, ax, ay, az, mx, my, mz, (now-lastUp)/1000000.0);
    }else{
      Update(gx, gy, gz, ax, ay, az, (now-lastUp)/1000000.0);      
    }
    lastUp = now;
  }
}

byte MahonyAHRS::floatEqZero(float a){
  if (abs(a) < 0.000001){
    return 1;
  }
  return 0;
}

/// <summary>
/// Algorithm AHRS update method. Requires only gyroscope and accelerometer data.
/// </summary>
/// <param name="gx">
/// Gyroscope x axis measurement in radians/s.
/// </param>
/// <param name="gy">
/// Gyroscope y axis measurement in radians/s.
/// </param>
/// <param name="gz">
/// Gyroscope z axis measurement in radians/s.
/// </param>
/// <param name="ax">
/// Accelerometer x axis measurement in any calibrated units.
/// </param>
/// <param name="ay">
/// Accelerometer y axis measurement in any calibrated units.
/// </param>
/// <param name="az">
/// Accelerometer z axis measurement in any calibrated units.
/// </param>
/// <param name="mx">
/// Magnetometer x axis measurement in any calibrated units.
/// </param>
/// <param name="my">
/// Magnetometer y axis measurement in any calibrated units.
/// </param>
/// <param name="mz">
/// Magnetometer z axis measurement in any calibrated units.
/// </param>
/// <remarks>
/// Optimised for minimal arithmetic.
/// </remarks> 
void MahonyAHRS::Update(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, float samplePeriod)
{
  float q1 = Quaternion[0], q2 = Quaternion[1], q3 = Quaternion[2], q4 = Quaternion[3];   // short name local variable for readability
  float norm;
  float hx, hy, bx, bz;
  float vx, vy, vz, wx, wy, wz;
  float ex, ey, ez;
  float pa, pb, pc;

  // Auxiliary variables to avoid repeated arithmetic
  float q1q1 = q1 * q1;
  float q1q2 = q1 * q2;
  float q1q3 = q1 * q3;
  float q1q4 = q1 * q4;
  float q2q2 = q2 * q2;
  float q2q3 = q2 * q3;
  float q2q4 = q2 * q4;
  float q3q3 = q3 * q3;
  float q3q4 = q3 * q4;
  float q4q4 = q4 * q4;   

  // Normalise accelerometer measurement
  norm = sqrt(ax * ax + ay * ay + az * az);
  if (norm == 0) return; // handle NaN
  norm = 1 / norm;        // use reciprocal for division
  ax *= norm;
  ay *= norm;
  az *= norm;

  // Normalise magnetometer measurement
  norm = sqrt(mx * mx + my * my + mz * mz);
  if (norm == 0) return; // handle NaN
  norm = 1 / norm;        // use reciprocal for division
  mx *= norm;
  my *= norm;
  mz *= norm;

  // Reference direction of Earth's magnetic field
  hx = 2 * mx * (0.5 - q3q3 - q4q4) + 2 * my * (q2q3 - q1q4) + 2 * mz * (q2q4 + q1q3);
  hy = 2 * mx * (q2q3 + q1q4) + 2 * my * (0.5 - q2q2 - q4q4) + 2 * mz * (q3q4 - q1q2);
  bx = sqrt((hx * hx) + (hy * hy));
  bz = 2 * mx * (q2q4 - q1q3) + 2 * my * (q3q4 + q1q2) + 2 * mz * (0.5 - q2q2 - q3q3);

  // Estimated direction of gravity and magnetic field
  vx = 2 * (q2q4 - q1q3);
  vy = 2 * (q1q2 + q3q4);
  vz = q1q1 - q2q2 - q3q3 + q4q4;
  wx = 2 * bx * (0.5f - q3q3 - q4q4) + 2 * bz * (q2q4 - q1q3);
  wy = 2 * bx * (q2q3 - q1q4) + 2 * bz * (q1q2 + q3q4);
  wz = 2 * bx * (q1q3 + q2q4) + 2 * bz * (0.5f - q2q2 - q3q3);  

  // Error is cross product between estimated direction and measured direction of gravity
  ex = (ay * vz - az * vy) + (my * wz - mz * wy);
  ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
  ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
  if (Ki > 0)
  {
    eInt[0] += ex;      // accumulate integral error
    eInt[1] += ey;
    eInt[2] += ez;
  }
  else
  {
    eInt[0] = 0.0;     // prevent integral wind up
    eInt[1] = 0.0;
    eInt[2] = 0.0;
  }

  // Apply feedback terms
  gx = gx + Kp * ex + Ki * eInt[0];
  gy = gy + Kp * ey + Ki * eInt[1];
  gz = gz + Kp * ez + Ki * eInt[2];

  // Integrate rate of change of quaternion
  pa = q2;
  pb = q3;
  pc = q4;
  q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5 * samplePeriod);
  q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5 * samplePeriod);
  q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5 * samplePeriod);
  q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5 * samplePeriod);

  // Normalise quaternion
  norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
  norm = 1.0f / norm;
  if (norm==0){
    
  }
  Quaternion[0] = q1 * norm;
  Quaternion[1] = q2 * norm;
  Quaternion[2] = q3 * norm;
  Quaternion[3] = q4 * norm;
}

/// <summary>
/// Algorithm IMU update method. Requires only gyroscope and accelerometer data.
/// </summary>
/// <param name="gx">
/// Gyroscope x axis measurement in radians/s.
/// </param>
/// <param name="gy">
/// Gyroscope y axis measurement in radians/s.
/// </param>
/// <param name="gz">
/// Gyroscope z axis measurement in radians/s.
/// </param>
/// <param name="ax">
/// Accelerometer x axis measurement in any calibrated units.
/// </param>
/// <param name="ay">
/// Accelerometer y axis measurement in any calibrated units.
/// </param>
/// <param name="az">
/// Accelerometer z axis measurement in any calibrated units.
/// </param>
void MahonyAHRS::Update(float gx, float gy, float gz, float ax, float ay, float az, float samplePeriod)
{
  float q1 = Quaternion[0], q2 = Quaternion[1], q3 = Quaternion[2], q4 = Quaternion[3];   // short name local variable for readability
  float norm;
  float vx, vy, vz;
  float ex, ey, ez;
  float pa, pb, pc;

  // Normalise accelerometer measurement
  norm = sqrt(ax * ax + ay * ay + az * az);
  if (norm == 0) return; // handle NaN
  norm = 1 / norm;        // use reciprocal for division
  ax *= norm;
  ay *= norm;
  az *= norm;

  // Estimated direction of gravity
  vx = 2.0f * (q2 * q4 - q1 * q3);
  vy = 2.0f * (q1 * q2 + q3 * q4);
  vz = q1 * q1 - q2 * q2 - q3 * q3 + q4 * q4;

  // Error is cross product between estimated direction and measured direction of gravity
  ex = (ay * vz - az * vy);
  ey = (az * vx - ax * vz);
  ez = (ax * vy - ay * vx);
  if (Ki > 0)
  {
    eInt[0] += ex;      // accumulate integral error
    eInt[1] += ey;
    eInt[2] += ez;
  }
  else
  {
    eInt[0] = 0.0;     // prevent integral wind up
    eInt[1] = 0.0;
    eInt[2] = 0.0;
  }

  // Apply feedback terms
  gx = gx + Kp * ex + Ki * eInt[0];
  gy = gy + Kp * ey + Ki * eInt[1];
  gz = gz + Kp * ez + Ki * eInt[2];

  // Integrate rate of change of quaternion
  pa = q2;
  pb = q3;
  pc = q4;
  q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * (0.5 * samplePeriod);
  q2 = pa + (q1 * gx + pb * gz - pc * gy) * (0.5 * samplePeriod);
  q3 = pb + (q1 * gy - pa * gz + pc * gx) * (0.5 * samplePeriod);
  q4 = pc + (q1 * gz + pa * gy - pb * gx) * (0.5 * samplePeriod);

  // Normalise quaternion
  norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
  norm = 1.0f / norm;
  Quaternion[0] = q1 * norm;
  Quaternion[1] = q2 * norm;
  Quaternion[2] = q3 * norm;
  Quaternion[3] = q4 * norm;
}

void MahonyAHRS::reset(){
  Quaternion[0] = 1;
    Quaternion[1] = 0;
      Quaternion[2] = 0;
        Quaternion[3] = 0;
        
  eInt[0] = eInt[1] = eInt[2] = 0;
}

void MahonyAHRS::printQuad(){
  Serial.println("MahonyAHRS");
  Serial.print(Quaternion[0]);
  Serial.print(" ");
  Serial.print(Quaternion[1]);
  Serial.print(" ");
  Serial.print(Quaternion[2]);
  Serial.print(" ");
  Serial.print(Quaternion[3]);
  Serial.println();
  Serial.flush();
}
