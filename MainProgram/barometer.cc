#include "barometer.h"

Barometer::Barometer() : Sensor(0x76) {}

bool Barometer::begin(){
    startPressure = getStartPressure();
}
void Barometer::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie les attributs pression et altitude

    pressure = 0; //temp
    altitude = 0; //temp 
}

double Barometer::getStartPressure(){
    return 0.0;
}

//accesseur
unsigned Barometer::getAltitude() const {
    return altitude;
}
