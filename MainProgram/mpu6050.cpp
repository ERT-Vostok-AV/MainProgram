#include "I2Cdev.h"
#include "mpu6050.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "mpuC6050.h" // not necessary if using MotionApps include file
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#include "Arduino.h"

/**Error codes:
 * 0 no error
 * devStatus Check DMP documentation
 */

MPU6050 mpuC(0x68);
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;
VectorFloat gravity; 
float angles[4], euler[3]; 

#define radToDeg 57.2958

Mpu6050::Mpu6050() : Sensor(0x68) {}
double gXOff, gYOff, gZOff, aXOff, aYOff, aZOff;
double currTimeM, prevTime, deltaTime;
// Ratio to convert radians to degrees.


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
    devStatus = mpuC.dmpInitialize();
  
    if(devStatus == 0){
      mpuC.CalibrateAccel(6);
      mpuC.CalibrateGyro(6);
      
      Serial.println(F("Enabling DMP.."));
      mpuC.setDMPEnabled(true);
      packetSize = mpuC.dmpGetFIFOPacketSize();
      return devStatus;
    } else {
      return devStatus;
    }
}

// Get the most recent values and stores them.
void Mpu6050::measure(){

    mpuC.resetFIFO();
    fifoCount = mpuC.getFIFOCount();
  
    while(fifoCount < packetSize) fifoCount = mpuC.getFIFOCount();
    
    // Gatter the latest measures in the fifo
    mpuC.getFIFOBytes(fifoBuffer, packetSize);

    
    float quat[4];
    quat[0] = (fifoBuffer[0] << 8 | fifoBuffer[1]) / 16384.0f;
    quat[1] = (fifoBuffer[4] << 8 | fifoBuffer[5]) / 16384.0f;
    quat[2] = (fifoBuffer[8] << 8 | fifoBuffer[9]) / 16384.0f;
    quat[3] = (fifoBuffer[12] << 8 | fifoBuffer[13]) / 16384.0f;
    for (int i = 0; i < 4; i++) if (quat[i] >= 2) quat[i] = -4 + quat[i];

    q.w = quat[0]; q.x = quat[1]; q.y = quat[2]; q.z = quat[3];
    quatToAngle();
}

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

void Mpu6050::printQuat(){
  quatToAngle();
  Serial.print(angles[0] ); Serial.print(" ");
  Serial.print(angles[1] ); Serial.print(" ");
  Serial.print(angles[2] ); Serial.print(" ");
  Serial.print(angles[3] ); Serial.println();
}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return (angles[0] * angles[1]) * RAD_TO_DEG;}
double Mpu6050::getRotY() {return (angles[0] * angles[2]) * RAD_TO_DEG;}
double Mpu6050::getRotZ() {return (angles[0] * angles[3]) * RAD_TO_DEG;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
