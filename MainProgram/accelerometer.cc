#include "accelerometer.h"



Accelerometer::Accelerometer() : Sensor(0x68) {}

void Accelerometer::measure() {
    long gForceX, gForceY, gForceZ, Tone, Ttwo, inter;
    Ttwo = millis();
    Wire.beginTransmission(0b1101000);//I2C adress of MPU
  Wire.write(0x3B); //Starting register for Accel data
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6);//Request Accel registers
  while(Wire.available() <6);
  accelX = Wire.read()<<8|Wire.read();//Store first data
  accelY = Wire.read()<<8|Wire.read();//Store middle data
  accelZ = Wire.read()<<8|Wire.read();//Store last data
    gForceX = accelX / 16384.0;//Divide by 16384.0 to know the force in Gs
gForceY = accelY / 16384.0;
gForceZ = accelZ / 16384.0;
    Tone = millis();
    inter = Ttwo - Tone;
    
    xAcc = (gForceX * inter)/1000;
    yAcc = (gForceY * inter)/1000;
    zAcc = (gForceZ * inter)/1000;
}

bool Accelerometer::begin() {
   Wire.beginTransmission(0b1101000);//I2C adress of MPU
  Wire.write(0x6B);//6B access
  Wire.write(0b00000000);//Setting SLEEP register to 0
  Wire.endTransmission();
  Wire.beginTransmission(0b1101000);//I2C adress of MPU
  Wire.write(0x1C);//Accessing the register 1B (Accelerometer configuration)
  Wire.write(0b00000000);
  Wire.endTransmission();
}

//accesseur
Vector<double> Accelerometer::getAcc() {
    measure();
    return {xAcc, yAcc, zAcc};
}
