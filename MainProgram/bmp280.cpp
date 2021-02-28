#include "bmp280.h"
#include <Adafruit_BMP280.h>

Bmp280::Bmp280() : Sensor(0x68) {}

Adafruit_BMP280 bmpAda;
int nbrStartPressure = 200; //nombre d'échantillons pour la calibration de pression
double arrondiAltitude;

bool Bmp280::begin() {
    //initialisation et calibration
    bool success = bmpAda.begin(i2cAddress);
    startPressure = getStartPressure();

    return success;
}

void Bmp280::measure(){
    //altitude
    arrondiAltitude = (double) bmpAda.readAltitude(startPressure); // arrondir l'altitude

    if(arrondiAltitude > 2048){
		arrondiAltitude = 2048;
	}
	else if (arrondiAltitude < 0){
		arrondiAltitude = 0;
	}

    altitude = arrondiAltitude;

    //temperature
    temperature = bmpAda.readTemperature(); // en °C
    
    if(temperature > 64){
		temperature = 64;
	}
	else if (temperature < -64){
		temperature = -64;
	}
}

double Bmp280::getStartPressure(){
    
	double moyennePression = 0;
	for(int i = 0; i<=nbrStartPressure; ++i){
		moyennePression += bmpAda.readPressure() / nbrStartPressure;
	}

    return moyennePression / 100; //division par 100 pour l'avoir en hP
}

//accesseurs
double Bmp280::getAlt() {
    return altitude;
}

double Bmp280::getTemp() {
    return temperature;
}