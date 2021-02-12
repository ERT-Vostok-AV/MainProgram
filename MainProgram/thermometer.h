#if !defined(THERMOMETER_H)
#define THERMOMETER_H

/*
extends sensor
A class that represents the thermometer aboard the rocket

*/

#include "sensor.h"

class Thermometer : public Sensor {

public:
    Thermometer();
    
    bool begin();
    
    int getTemperature();

private:
    virtual void measure() override;

    int temperature; //ranges from -64 to 64 °C
};

#endif // THERMOMETER_H