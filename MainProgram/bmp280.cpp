#include "bmp280.h"
#include <Adafruit_BMP280.h>

Bmp280::Bmp280() : Sensor(0x68) {}

Adafruit_BMP280 bmp;
int nbrStartPressure = 200; //nombre d'échantillons pour la calibration de pression
int arrondiAltitude;

bool Bmp280::begin() {
    //initialisation et calibration
    bool success = bmp.begin(i2cAddress);
    startPressure = getStartPressure();

    return success;
}

void Bmp280::measure(){
    //altitude
    arrondiAltitude = (int) bmp.readAltitude(startPressure); // arrondir l'altitude

    if(arrondiAltitude > 2048){
		arrondiAltitude = 2048;
	}
	else if (arrondiAltitude < 0){
		arrondiAltitude = 0;
	}

    altitude = arrondiAltitude;


    //temperature
    temperature = bmp.readTemperature(); // en °C
}

double Bmp280::getStartPressure(){
    
	double moyennePression = 0;
	for(int i = 0; i<=nbrStartPressure; ++i){
		moyennePression += bmp.readPressure() / nbrStartPressure;
	}

    return moyennePression / 100; //division par 100 pour l'avoir en hP
}

//accesseurs
int* Bmp280::getAltAndTemp() {
    measure();
    int* results[] = {altitude, temperature};
    return results;
}