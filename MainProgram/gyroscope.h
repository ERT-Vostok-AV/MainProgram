#if !defined(GYROSCOPE_H)
#define GYROSCOPE_H

/*
extends sensor
A class that represents the gyroscope a board the rocket

*/

#include <Vector.h>
#include "sensor.h"

class Gyroscope : public Sensor {

public:
    Gyroscope();
    
    bool begin();
    
    Vector<double> getRot();

private:
    virtual void measure() override;
    // 16 bit askip et en [g]
    double xRot;
    double yRot;
    double zRot;
};

#endif // GYROSCOPE_H
