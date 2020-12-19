#include "gyroscope.h"

Gyroscope::Gyroscope(int i2cAddress) : Sensor(i2cAddress) {
    //initialisation calibration ici
    measure();
}

void Gyroscope::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie l'attribut temperature

    //temp
    xRot = 0;
    yRot = 0;
    zRot = 0;
    
}

//Rotesseur
double Gyroscope::getXRot() const {
    return xRot;
}

double Gyroscope::getYRot() const {
    return yRot;
}

double Gyroscope::getZRot() const {
    return zRot;
}
