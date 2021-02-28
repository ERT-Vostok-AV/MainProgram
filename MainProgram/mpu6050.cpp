#include "mpu6050.h"

#include <Adafruit_MPU6050.h>

Mpu6050::Mpu6050() : Sensor(0x76) {}

bool Mpu6050::begin(){

}

void Mpu6050::measure(){

}

//accesseurs
double* Mpu6050::getRotAndVel() { //retourne un array de double
    measure();
    double results[] = {xRot, yRot, zRot, xVel, yVel, zVel};
    return results;
}