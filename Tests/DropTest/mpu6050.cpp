#include "I2Cdev.h"
#include "mpu6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
#include "Arduino.h"

#include <stdio.h>

// Address of the MPU on the i2c bus
#define SENSOR_ADDR 0x68
// Radian to degree factor
#define radToDeg 57.2958
// Value of the scale for the accel
#define PRECISION 2048.0f; //16384.0f

// MPU object from the i2cdev lib
MPU6050 mpuC(SENSOR_ADDR);
// Size of a packet
uint16_t packetSize;
// Size of the fifo 
uint16_t fifoCount;
// Contains the value of the fifo
uint8_t fifoBuffer[64];
// Contains the orientation as a quaternion
Quaternion q;   
// Contains the gravity vector
VectorFloat gravity;
// Contains the veloctiy
VectorInt16 vel; 
// Contains the orientation as axis-anlge et euler representations
float angles[4], euler[3]; 
// Hold the values of the velocity and accel of x y and z
float velX, velY, velZ, accX, accY, accZ;
double currTimeM, prevTimeM, deltaTimeM;

Mpu6050::Mpu6050() : Sensor(SENSOR_ADDR) {}

/*
 * Initializes the MPU sensor
 */
int Mpu6050::begin(){
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
    #endif

    mpuC.initialize();
  
    Serial.println(F("Testing connection.."));
    Serial.println(mpuC.testConnection() ? F("Success") : F("Failed"));
  
    Serial.println(F("Initializing DMP..."));
    uint8_t devStatus = mpuC.dmpInitialize();
  
    if(devStatus == 0){
      mpuC.CalibrateAccel(6);
      mpuC.CalibrateGyro(6);
      
      Serial.println(F("Enabling DMP.."));
      mpuC.setDMPEnabled(true);
      packetSize = mpuC.dmpGetFIFOPacketSize();
      
      mpuC.setFullScaleAccelRange(MPU6050_ACCEL_FS_16);
      mpuC.setFullScaleGyroRange(MPU6050_GYRO_FS_2000);

      currTimeM = millis();
      return devStatus;
    } else {
      return devStatus;
    }
}

/*
 * Reads the most recent values from the MPU and stores them 
 */
void Mpu6050::measure(){

    // Empty the fifo
    mpuC.resetFIFO();

    //Get the size of the fifo
    fifoCount = mpuC.getFIFOCount();

    // Wait for the fifo to fill until reached the size of a packet
    while(fifoCount < packetSize) fifoCount = mpuC.getFIFOCount();

    //Fills the fifo buffer according to the packet size
    mpuC.getFIFOBytes(fifoBuffer, packetSize);
    mpuC.dmpGetQuaternion(&q, fifoBuffer);

    // Manually fetching velocity values using I2C
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(SENSOR_ADDR, 6, true);
    accX = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;
    accY = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;
    accZ = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;

    prevTimeM = currTimeM;
    currTimeM = millis();
    deltaTimeM = currTimeM - prevTimeM;
    // Extract velocity from accel (basically integral)
    velX = accX * deltaTimeM;
    velY = accY * deltaTimeM;
    velZ = accZ * deltaTimeM;
}

/*
 * Convert from quaternion to axis angle
 * @param res : the float table in which the result should be put into
 */
int Mpu6050::quatToAngle(float* res){
  float angle = acos(q.w) * 2;
  float dividend = sqrt(1 - (q.w * q.w));
  if(dividend < 0.001){ // to avoid division by 0
    angles[1] = 1;
    angles[2] = 0;
    angles[3] = 0;
  } else {
    angles[1] = q.x / dividend;
    angles[2] = q.y / dividend;
    angles[3] = q.z / dividend;
  }
  angles[0] = angle;
}

/*
 * STUB 
 */
void Mpu6050::printQuat(){
  quatToAngle(angles);  
  //Serial.printf("%u\n",mpuC.getFullScaleAccelRange());

  Serial.print(angles[0] ); Serial.print(" ");
  Serial.print(angles[1] ); Serial.print(" ");
  Serial.print(angles[2] ); Serial.print(" ");
  Serial.print(angles[3] ); Serial.println();

}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return (angles[0] * -angles[1]);}
double Mpu6050::getRotY() {return (angles[0] * angles[3]);}
double Mpu6050::getRotZ() {return (angles[0] * angles[2]);}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
