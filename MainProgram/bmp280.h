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
    
    bool begin();

    int* getAltAndTemp();

private:
    virtual void measure() override;
    double getStartPressure();
    double startPressure;

    int altitude; // ranges from 0 to 2048 m
    int temperature; //ranges from -64 to 64 °C
};

#endif // BMP280_H

