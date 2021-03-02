#include "mpu6050.h"
#include <MPU6050_light.h>
#include "Arduino.h"

MPU6050 mpuAda(Wire);

#define radToDeg 57.2958

Mpu6050::Mpu6050() : Sensor(0x68) {}
double gXOff, gYOff, gZOff, aXOff, aYOff, aZOff;
// Ratio to convert radians to degrees.


bool Mpu6050::begin(){
}

// Get the most recent values and stores them.
void Mpu6050::measure(){
}

// Compute offset by averarging over a 100 measures.
void Mpu6050::getOffsets(){
}

//Getter for the velocity and orientation
double Mpu6050::getRotX() {return degX;}
double Mpu6050::getRotY() {return degY;}
double Mpu6050::getRotZ() {return degZ;}

double Mpu6050::getVelX() {return velX;}
double Mpu6050::getVelY() {return velY;}
double Mpu6050::getVelZ() {return velZ;}
