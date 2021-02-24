#include "barometer.h"
#include <Wire.h>
//#include <SPI.h> je sais pas si je vais en avoir besoin.
#include <Adafruit_BMP280.h>//librairie à télécharger https://github.com/adafruit/Adafruit_BMP280_Library
#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10
Adafruit_BMP280 bmp;
int Nbrstartpressure = 10;
int arrondialtitude;
bool dudebut;
Barometer::Barometer() : Sensor(0x76) {}


// dans librairie adresse par défault est 0x77 et Chip ID est 0x58

bool Barometer::begin(){
    //TODO initialisation et calibration
	dudebut = bmp.begin();//possible qu'il faut donner l'adresse et le chip id pour que la fonction fonctionne et elle renvoie True si tout va bien
	startPressure = getStartPressure();
	return dudebut;
}

void Barometer::measure() {
    //TODO Récupère les données brutes, les traites si besoin
    //et modifie les attributs pression et altitude
	arrondialtitude = int(bmp.readAltitude(startPressure/100)); // arrondir l'altitude et la division par 100 pour passer en hPa.
	
	if(arrondialtitude>2048){
		arrondialtitude=2048;
	}
	
	if(arrondialtitude<0){
		arrondialtitude=0;
	}
    pressure = bmp.readPressure();//la fonction renvoie du float
    altitude = arrondialtitude;//calcule altitude à partir de la pression de base
}

double Barometer::getStartPressure(){
    
	double moyennepression = 0;
	for(int i = 0,i<=Nbrstartpressure,++i){
	
	moyennepression += bmp.readPressure()/Nbrstartpressure;
		
	}

    return moyennepression;
}

//accesseur
unsigned Barometer::getAltitude() {
    measure();
    return altitude;
}

