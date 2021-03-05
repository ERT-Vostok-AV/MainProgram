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
Quaternion q;   
VectorFloat gravity; 
float ypr[3]; 

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
  
    mpuC.getFIFOBytes(fifoBuffer, packetSize);
  
    mpuC.dmpGetQuaternion(&q, fifoBuffer);
    mpuC.dmpGetGravity(&gravity, &q);
    mpuC.dmpGetYawPitchRoll(ypr, &q, &gravity);
  
    degZ = ypr[0] * 180/M_PI;
    degY = ypr[1] * 180/M_PI;
    degZ = ypr[2] * 180/M_PI;

    
}


//Getter for the velocity and orientation
double Mpu6050::getRotX() {return degX;}
double Mpu6050::getRotY() {return degY;}
double Mpu6050::getRotZ() {return degZ;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
