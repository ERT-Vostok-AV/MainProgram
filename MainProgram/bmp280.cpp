#include "bmp280.h"
#include <Adafruit_BMP280.h>

/** Error codes:
 * 0 no error
 * 1 bmp sensor error
 */

Adafruit_BMP280 bmpAda;
int nSamples = 100; //nombre d'échantillons pour la calibration de pression
double arrondiAltitude;


Bmp280::Bmp280() : Sensor(0x77) {}

int Bmp280::begin() {
    //initialisation et calibration
    if(bmpAda.begin(i2cAddress)){
      bmpAda.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                        Adafruit_BMP280::STANDBY_MS_1); 
      getStartPressure();
      Serial.print("Start pressure: "); Serial.println(startPressure);
      return 0;
    }
    return 1;
}

void Bmp280::measure(){
    //Get the altitude and constrains it to the interval [0, 2048]
    altitude = clip(bmpAda.readAltitude(startPressure), 0.0, 2048.0);
    // Get the temperature and constrains it to the interval [-64, 64]
    temperature = clip(bmpAda.readTemperature(), -64.0, 64.0);
}

void Bmp280::getStartPressure(){
    
	startPressure = 0;
	for(int i = 0; i < nSamples; i++){
		startPressure += bmpAda.readPressure();
	}

  startPressure /= (nSamples * 100); //division par 100 pour l'avoir en hP
}

//accesseurs
double Bmp280::getAlt() {
    return altitude;
}

double Bmp280::getTemp() {
    return temperature;
}

/**
 * Constrains the value val withing the value lo and hi.
 */
double Bmp280::clip(double val, double lo, double hi){
  if(val < lo){
    return lo;
  } else if(val > hi){
    return hi;
  } else {
    return val;
  }
}
