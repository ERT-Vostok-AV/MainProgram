#include "thermometer.h"

Thermometer::Thermometer() : Sensor(0x68) {}


void Thermometer::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie l'attribut de la température

    //Codez ici

    //(remplacer le 0 par la valeur calculée)
    temperature = 0;
}

bool Thermometer::begin() {
    //TODO initialisation et calibration
}

//accesseur
int Thermometer::getTemperature() {
    measure();
    return temperature;
}
