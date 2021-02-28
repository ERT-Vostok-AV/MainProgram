#include "thermometer.h"
#include <Adafruit_BMP280.h>


Thermometer::Thermometer() : Sensor(0x68) {}

Adafruit_BMP280 bmpThermo;

void Thermometer::measure() {
    
    temperature =  bmpThermo.readTemperature();// en °C
}

bool Thermometer::begin() {
	
	return bmpThermo.begin(i2cAddress);
}

// dans librairie adresse par défault est 0x77 et Chip ID est 0x58

int Thermometer::getTemperature() {
    measure();
    return temperature;
}
