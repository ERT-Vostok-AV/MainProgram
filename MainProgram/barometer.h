#if !defined(BAROMETER_H)
#define BAROMETER_H

/*
extends sensor
A class that represents the barometer aboard the rocket

*/

#include "sensor.h"

class Barometer : public Sensor {

public:
    Barometer(int i2cAddress);
    virtual void measure() override;
    
    int getAltitude() const;

private:
    double pressure;
    unsigned altitude; // ranges from 0 to 2048 m


};

#endif // BAROMETER_H