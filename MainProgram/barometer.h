#if !defined(BAROMETER_H)
#define BAROMETER_H

/*
extends sensor
A class that represents the barometer aboard the rocket

*/

#include "sensor.h"

class Barometer : public Sensor {

public:
    Barometer();
    
    bool begin();

    unsigned getAltitude();

private:
    virtual void measure() override;
    double getStartPressure();
    double pressure, startPressure;
    unsigned altitude; // ranges from 0 to 2048 m  /!\ normalizer /!\ f(x) = o si x<0, 2048 si x > 2048, x si x e[0,2048]


};

#endif // BAROMETER_H

