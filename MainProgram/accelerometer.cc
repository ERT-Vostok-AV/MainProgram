#include "accelerometer.h"



Accelerometer::Accelerometer(int i2cAddress) : Sensor(i2cAddress) {
    //initialisation calibration ici
    measure();
}

void Accelerometer::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie les attributs accélération

    //temp
    xAcc = 0;
    yAcc = 0;
    zAcc = 0;
    
}

//accesseur
double Accelerometer::getXAcc() const {
    return xAcc;
}

double Accelerometer::getYAcc() const {
    return yAcc;
}

double Accelerometer::getZAcc() const {
    return zAcc;
}
