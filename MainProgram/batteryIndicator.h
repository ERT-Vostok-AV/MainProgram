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

    int begin();

    virtual void measure() override;
    
    double getBatteryLevel();

private:
    int batteryLevel; // ranges from 0 to 100 %
    int pin;
    // maxV est le voltage max qu'une batterie pleine peu atteindre avec les résistance r1 et r2
    // et maxBin et le mapping de maxV [0, 3.3] sur [0, 1024];
    double maxV, maxBin;
    double r1 = 80.3; // k Ohm
    double r2 = 325.8; // k Ohm
};



#endif // BATTERY_INDICATOR_H
