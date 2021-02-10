#include "batteryIndicator.h"

BatteryIndicator::BatteryIndicator() : Sensor(0x000){ //No i2C address for the battery indicator
    //initialisation calibration ici
    measure();
}

void BatteryIndicator::measure() {
    //Récupère les données brutes, les traites si besoin
    //et modifie l'attribut batteryLevel

    batteryLevel = 0; //temp
}

//accesseur
unsigned BatteryIndicator::getBatteryLevel() const {
    return batteryLevel;
}
