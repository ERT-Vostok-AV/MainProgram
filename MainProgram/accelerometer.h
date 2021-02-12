#if !defined(ACCELEROMETER_H)
#define ACCELEROMETER_H

/*
extends sensor
A class that represents the accelerometer a board the rocket

*/

#include <vector>
#include "sensor.h"

class Accelerometer : public Sensor {

public:
    Accelerometer();

    bool begin();
    
    std::vector<double> getAcc();

private:
    virtual void measure() override;
    // 16 bit askip et en [g]
    double xAcc;
    double yAcc;
    double zAcc;
};

#endif // ACCELEROMETER_H