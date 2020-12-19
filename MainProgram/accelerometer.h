/*
extends sensor
A class that represents the accelerometer a board the rocket

*/

#include "sensor.h"

class Accelerometer : public Sensor {

public:
    Accelerometer(int i2cAddress);
    virtual void measure() override;
    
    double getXAcc() const;
    double getYAcc() const;
    double getZAcc() const;

private:
    // 16 bit askip et en [g]
    double xAcc;
    double yAcc;
    double zAcc;
};