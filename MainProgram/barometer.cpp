#include "barometer.h"

#include <Adafruit_BMP280.h>//librairie à télécharger https://github.com/adafruit/Adafruit_BMP280_Library


Barometer::Barometer() : Sensor(0x76) {}

Adafruit_BMP280 bmp;
int nbrStartPressure = 200;
int arrondiAltitude;

// dans librairie adresse par défault est 0x77 et Chip ID est 0x58

bool Barometer::begin(){
    //TODO initialisation et calibration
	
	bool success = bmp.begin(i2cAddress);//possible qu'il faut donner l'adresse et le chip id pour que la fonction fonctionne et elle renvoie True si tout va bien
	startPressure = getStartPressure();

	return success;
}

void Barometer::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie les attributs pression et altitude
	arrondiAltitude = int(bmp.readAltitude(startPressure)); // arrondir l'altitude et la division par 100 pour passer en hPa.
	
	if(arrondiAltitude > 2048){
		arrondiAltitude = 2048;
	}
	else if (arrondiAltitude < 0){
		arrondiAltitude = 0;
	}

    altitude = arrondiAltitude; //calcule altitude à partir de la pression de base
}

double Barometer::getStartPressure(){
    
	double moyennePression = 0;
	for(int i = 0; i<=nbrStartPressure; ++i){
		moyennePression += bmp.readPressure() / nbrStartPressure;
	}

    return moyennePression / 100; //
}

//accesseur
unsigned Barometer::getAltitude() {
    measure();
    return altitude;
}

