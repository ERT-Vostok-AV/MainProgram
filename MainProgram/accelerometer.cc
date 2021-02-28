#include "accelerometer.h"



Accelerometer::Accelerometer() : Sensor(0x68) {}

void Accelerometer::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie les attributs de accéleration

    //Codez ici

    //(remplacer les 0 par les valeurs calculées)
    xAcc = 0;
    yAcc = 0;
    zAcc = 0;
}

bool Accelerometer::begin() {
    //TODO initialisation et calibration
}

//accesseur
Vector<double> Accelerometer::getAcc() {
    measure();
    return {xAcc, yAcc, zAcc};
}