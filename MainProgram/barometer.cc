#include "barometer.h"

Barometer::Barometer() : Sensor(0x76) {}

bool Barometer::begin(){
    //TODO initialisation et calibration
    startPressure = getStartPressure();
}

void Barometer::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie les attributs pression et altitude

    //Codez ici

    //(remplacer les 0 par les valeurs calculées)
    pressure = 0;
    altitude = 0;
}

double Barometer::getStartPressure(){
    //TODO trouver une pression de base à l'aide
    //d'une moyenne sur plusieurs valeurs par exemple
    return 0.0;
}

//accesseur
unsigned Barometer::getAltitude() {
    measure();
    return altitude;
}
