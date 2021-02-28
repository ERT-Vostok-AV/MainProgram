#if !defined(MPU6050_H)
#define MPU6050_H

/*
extends sensor
A class that represents the accelerometer a board the rocket

*/

#include "sensor.h"

class Mpu6050 : public Sensor {

public:
    Mpu6050();

    bool begin();
    
    double* getRotAndVel();

private:
    virtual void measure() override;
    // 16 bit askip et en [m/s]
    double xVel, yVel, zVel; //velocity
    double xRot, yRot, zRot; //rotation
};

#endif // MPU6050_H
