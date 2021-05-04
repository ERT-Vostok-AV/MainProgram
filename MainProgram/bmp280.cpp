#include "bmp280.h"
#include <Adafruit_BMP280.h>

// Address of the sensor on the I2C bus
#define SENSOR_ADDR 0x77
// Number of samples used for the mean of the initial pressure function
#define N_SAMPLES 100
// BMP object from adafruit library
Adafruit_BMP280 bmpAda;

Bmp280::Bmp280() : Sensor(SENSOR_ADDR) {}

/*
 * @brief Initialise the BMP
 */
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

/*
 * @brief Reads the values from the BMP, clip them to their interval and store
 */
void Bmp280::measure(){
    //Get the altitude and constrains it to the interval [-2048, 2048]
    altitude = clip(bmpAda.readAltitude(startPressure), -2048, 2048.0);
    // Get the temperature and constrains it to the interval [-64, 64]
    temperature = clip(bmpAda.readTemperature(), -64.0, 64.0);
}

/**
 * @brief Aggregate over a 100 read the current pressure to produce a mean pressure
 *        to be used before the fligt
 */
void Bmp280::getStartPressure(){
    
	startPressure = 0;
	for(int i = 0; i < N_SAMPLES; i++){
		startPressure += bmpAda.readPressure();
	}

  startPressure /= (N_SAMPLES * 100); //division par 100 pour l'avoir en hP
}

/*
 * @brief Getter for the altitude
 * @return: altitude as a double
 */
double Bmp280::getAlt() {
    return altitude;
}

/*
 * @brief Getter for the temperature
 * @return the temperature as a double
 */
double Bmp280::getTemp() {
    return temperature;
}

/**
 * @brief Constrains the value val withing the value lo and hi.
 * @param val double : value to constrain 
 * @param lo  double : lower bound
 * @param hi  double : upper bound
 * @return the constrained value 
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
