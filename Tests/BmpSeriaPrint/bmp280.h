#if !defined(BMP280_H)
#define BMP280_H

/*
extends sensor
A class that represents the bmp280 aboard the rocket

*/

#include "sensor.h"

class Bmp280 : public Sensor {

public:
    Bmp280();
    
    int begin();

    virtual void measure() override;

    double getAlt();
    double getTemp();

private:
    void getStartPressure();
    float startPressure;

    double altitude; // ranges from 0 to 2048 m
    double temperature; //ranges from -64 to 64 °C

    double clip(double val, double lo, double hi); // Clip the value val in [lo, hi]
};

#endif // BMP280_H
