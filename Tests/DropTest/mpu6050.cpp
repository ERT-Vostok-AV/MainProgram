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
  
    //teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
    
    mpuC.resetFIFO();

    fifoCount = mpuC.getFIFOCount();
  
    while(fifoCount < packetSize) fifoCount = mpuC.getFIFOCount();
    
    mpuC.getFIFOBytes(fifoBuffer, packetSize);

    
    float quat[4];
    quat[0] = (fifoBuffer[0] << 8 | fifoBuffer[1]) / 16384.0f;
    quat[1] = (fifoBuffer[4] << 8 | fifoBuffer[5]) / 16384.0f;
    quat[2] = (fifoBuffer[8] << 8 | fifoBuffer[9]) / 16384.0f;
    quat[3] = (fifoBuffer[12] << 8 | fifoBuffer[13]) / 16384.0f;
    for (int i = 0; i < 4; i++) if (quat[i] >= 2) quat[i] = -4 + quat[i];

    q.w = quat[0]; q.x = quat[1]; q.y = quat[2]; q.z = quat[3];
    
    //mpuC.dmpGetQuaternion(&q, fifoBuffer);
    
    
    
    /**
    teapotPacket[2] = fifoBuffer[0];
    teapotPacket[3] = fifoBuffer[1];
    teapotPacket[4] = fifoBuffer[4];
    teapotPacket[5] = fifoBuffer[5];
    teapotPacket[6] = fifoBuffer[8];
    teapotPacket[7] = fifoBuffer[9];
    teapotPacket[8] = fifoBuffer[12];
    teapotPacket[9] = fifoBuffer[13];
    */

    /*
    degX = euler[0] * 180/M_PI;
    degY = euler[1] * 180/M_PI;
    degZ = euler[2] * 180/M_PI;
    */
    
    //degX = ypr[0];
    //degY = ypr[1];
    //degZ = ypr[2];

}

void Mpu6050::quatToAngle(){
  float angle = acos(q.w) * 2;
  float dividend = sqrt(1 - (q.w * q.w));
  if(dividend < 0.001){
    angles[1] = 1;
    angles[2] = 0;
    angles[3] = 0;
  } else {
    angles[1] = q.x / dividend;
    angles[2] = q.y / dividend;
    angles[3] = q.z / dividend;
  }
  angles[0] = angle;
    
  /**
  // Roll calculation
  double tanUp = 2 * (q.w * q.x + q.y * q.z);
  double tanDown = 1 - 2 * (q.x * q.x + q.y * q.y);
  ypr[2] = atan2(tanUp, tanDown);

  // Yaw calculation
  tanUp = 2 * (q.w * q.z + q.y * q.x);
  tanDown = 1 - 2 * (q.y * q.y + q.z * q.z);
  ypr[0] = atan2(tanUp, tanDown);

  // Pitch calculation
  double sinV = 2 * (q.w * q.y + q.x * q.z);
  if (abs(sinV) >= 1){
    ypr[1] = copysign(M_PI / 2 , sinV);
  } else 
    ypr[1] = asin(sinV);
  */
}

void Mpu6050::printQuat(){
  quatToAngle();
  Serial.print(angles[0] ); Serial.print(" ");
  Serial.print(angles[1] ); Serial.print(" ");
  Serial.print(angles[2] ); Serial.print(" ");
  Serial.print(angles[3] ); Serial.println();

  /*
  Serial.print((fifoBuffer[0] << 8 | fifoBuffer[1]) / 16384.0f); Serial.print(" ");
  Serial.print((fifoBuffer[4] << 8 | fifoBuffer[5]) / 16384.0f); Serial.print(" ");
  Serial.print((fifoBuffer[8] << 8 | fifoBuffer[9]) / 16384.0f); Serial.print(" ");
  Serial.print((fifoBuffer[12] << 8 | fifoBuffer[13]) / 16384.0f); Serial.println("");
  */
}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return degX;}
double Mpu6050::getRotY() {return degY;}
double Mpu6050::getRotZ() {return degZ;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
