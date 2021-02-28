#include "accelerometer.h"
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;
long gForceX, gForceY, gForceZ, Timer, inter;
sensor_event_t accel;

Accelerometer::Accelerometer() : Sensor(0x68) {}

bool Accelerometer::begin() {
  bool success = mpu.begin(i2cAddress);
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  return success;

}

void Accelerometer::measure() {
   

  accelX = Wire.read()<<8|Wire.read();//Store first data
  accelY = Wire.read()<<8|Wire.read();//Store middle data
  accelZ = Wire.read()<<8|Wire.read();//Store last data

  gForceX = accelX / 16384.0;//Divide by 16384.0 to know the force in Gs
  gForceY = accelY / 16384.0;
  gForceZ = accelZ / 16384.0;

  inter = Timer - millis();
  Timer = millis();
    
  xVel = (gForceX * inter)/1000;
  yVel = (gForceY * inter)/1000;
  zVel = (gForceZ * inter)/1000;
}



//accesseur
double* Accelerometer::getVelocity() { //retourne un array de double
    measure();
    return {xVel, yVel, zVel};
}

