#include "barometer.h"

Barometer::Barometer(int i2cAddress) : Sensor(i2cAddress) {
    //initialisation calibration ici
    measure();
}

void Barometer::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie les attributs pression et altitude

    pressure = 0; //temp
    altitude = 0; //temp 
}

//accesseur
unsigned Barometer::getAltitude() const {
    return altitude;
}
