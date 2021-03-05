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
    
    double getRotX();
    double getRotY();
    double getRotZ();
    double getVelX();
    double getVelY();
    double getVelZ();
    
    virtual void measure() override;
    
private:
    // 16 bit askip et en [m/s]
    double velX, velY, velZ; //velocity
    double degX, degY, degZ; //rotation
};

#endif // MPU6050_H
