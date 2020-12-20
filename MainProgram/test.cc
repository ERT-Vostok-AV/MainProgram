#include <iostream>

#include "barometer.h"
#include "thermometer.h"
#include "accelerometer.h"
#include "gyroscope.h"

#include "sensor.h"

using namespace std;

int main(int argc, char const *argv[])
{
    //pas les bonnes addresses c'est un test
    Barometer baro(0x123);
    Thermometer thermo(0x456);
    Accelerometer accel(0x345);
    Gyroscope gyro(0x666);

    cout << "altitude : " << baro.getAltitude() << endl;
    cout << "temperature : " << thermo.getTemperature() << endl;
    cout << "acceleration :" << " x=" << accel.getXAcc() << " y=" << accel.getYAcc() << " z=" << accel.getZAcc() << endl;
    cout << "orientation :" << " Rx=" << gyro.getXRot() << " Ry=" << gyro.getYRot() << " Rz=" << gyro.getZRot() << endl;
    
    return 0;
}
