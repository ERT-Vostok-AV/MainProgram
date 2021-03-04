#if !defined(BATTERY_INDICATOR_H)
#define BATTERY_INDICATOR_H

/*
extends sensor
A class that represents the battery indicator aboard the rocket

*/

#include "sensor.h"

class BatteryIndicator : public Sensor {
public:
    BatteryIndicator(int pinC);

    bool begin();

    virtual void measure() override;
    
    double getBatteryLevel();

private:
    int batteryLevel; // ranges from 0 to 100 %
    int pin;
};



#endif // BATTERY_INDICATOR_H
