
#include <Wire.h>
#include "mpu6050.h"


void impact_movement_init() {
  Wire.begin();
  mpu6050_initialize();

  
}

void impact_movement_update() {
  

Serial.print(".");
mpu6050_Accel_Values();
  mpu6050_Gyro_Values();
  
  
float norm_a = (float)(fabs(accelRaw[XAXIS]))+(float)(fabs(accelRaw[YAXIS]))+(float)(fabs(accelRaw[ZAXIS]));
float norm_g = (float)(fabs(gyroRaw[XAXIS]))+(float)(fabs(gyroRaw[YAXIS]))+(float)(fabs(gyroRaw[ZAXIS]));
Serial.println(norm_a);
  if(norm_a > 8000 && _light_st == light_on) {
    _light_rq = light_crash;
  }
  if(norm_g > 2000 && norm_g < 3000 && _light_st == light_on && _light_rq == light_req_done ) {
    _light_rq = light_swing_low;
  }
  else if(norm_g>=3000 && _light_st == light_on && _light_rq == light_req_done) {
    _light_rq = light_swing_fast;
  }
}
