#if !defined(MPU6050_H)
#define MPU6050_H

/*
extends sensor
A class that represents the mpu6050 aboard the rocket

*/

#include "sensor.h"

class Mpu6050 : public Sensor {

public:
    Mpu6050();

    int begin();

    void printQuat();
    double getRotA();
    double getRotX();
    double getRotY();
    double getRotZ();
    double getVelX();
    double getVelY();
    double getVelZ();

    void getAngle(float* res);
    
    virtual void measure() override;
    
private:
    int quatToAngle(float* res);
    // 16 bit et en [m/s]
    double velX, velY, velZ; //velocity

    double rotA, rotX, rotY, rotZ; //rotation
};

#endif // MPU6050_H
