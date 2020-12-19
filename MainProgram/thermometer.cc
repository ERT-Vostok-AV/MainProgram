#include "thermometer.h"

Thermometer::Thermometer(int i2cAddress) : Sensor(i2cAddress) {
    //initialisation calibration ici
    measure();
}


void Thermometer::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie l'attribut temperature

    temperature = 0; //temp
}


//accesseur
int Thermometer::getTemperature() const {
    return temperature;
}
