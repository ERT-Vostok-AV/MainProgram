#if !defined(ACCELEROMETER_H)
#define ACCELEROMETER_H

/*
extends sensor
A class that represents the accelerometer a board the rocket

*/

#include <Vector.h>
#include "sensor.h"

class Accelerometer : public Sensor {

public:
    Accelerometer();

    bool begin();
    
    Vector<double> getAcc();

private:
    virtual void measure() override;
    // 16 bit askip et en [g]
    double xVel, yVel, zVel; //velocity
};

#endif // ACCELEROMETER_H
