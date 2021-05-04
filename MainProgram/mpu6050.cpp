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

Mpu6050::Mpu6050() : Sensor(SENSOR_ADDR) {}

/*
 * @brief Initializes the MPU sensor
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

      mpuC.setXAccelOffset(-2505.74);
      mpuC.setYAccelOffset(-1383.35);
      mpuC.setZAccelOffset(1254.78);
      mpuC.setXGyroOffset(-79.43);
      mpuC.setXGyroOffset(21.13);
      mpuC.setXGyroOffset(6.96);
      
      Serial.println(F("Enabling DMP.."));
      mpuC.setDMPEnabled(true);
      packetSize = mpuC.dmpGetFIFOPacketSize();
      return devStatus;
    } else {
      return devStatus;
    }
}

/*
 * @brief Reads the most recent values from the MPU and stores them 
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

    quatToAngle();

    // Manually fetching acceleration values using I2C
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(SENSOR_ADDR, 6, true);
    accelX = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;
    accelY = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;
    accelZ = (int16_t)(Wire.read() << 8| Wire.read()) / PRECISION;
}

/*
 * @brief Convert from quaternion to axis angle in the local axis-angle array
 */
void Mpu6050::quatToAngle(){
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

//Getter for the acceleration and orientation
double Mpu6050::getRotA() {return angles[0];}
double Mpu6050::getRotX() {return angles[1];}
double Mpu6050::getRotY() {return angles[2];}
double Mpu6050::getRotZ() {return angles[3];}

double Mpu6050::getAccelX() {return accelX;}
double Mpu6050::getAccelY() {return accelY;}
double Mpu6050::getAccelZ() {return accelZ;}
