/*
extends sensor
A class that represents the gyroscope a board the rocket

*/

#include "sensor.h"

class Gyroscope : public Sensor {

public:
    Gyroscope(int i2cAddress);
    virtual void measure() override;
    
    double getXRot() const;
    double getYRot() const;
    double getZRot() const;

private:
    // 16 bit askip et en [g]
    double xRot;
    double yRot;
    double zRot;
};