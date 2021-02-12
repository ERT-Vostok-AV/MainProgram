#include "gyroscope.h"

Gyroscope::Gyroscope() : Sensor(0x76) {}

void Gyroscope::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie les attributs de rotation

    //Codez ici

    //(remplacer les 0 par les valeurs calculées)
    xRot = 0;
    yRot = 0;
    zRot = 0;
}

bool Gyroscope::begin() {
    //TODO initialisation et calibration
}

//Accesseur
std::vector<double> Gyroscope::getRot() {
    measure();
    return {xRot, yRot, zRot};
}

