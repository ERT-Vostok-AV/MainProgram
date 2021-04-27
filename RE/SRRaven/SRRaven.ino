#include <Adafruit_BMP280.h>

#define INIT_SAMPLES 200
#define TRIGGER_PIN 13 // CHANGE THAT
#define BURN_TIME 500 //ms
#define TRIGGER_ALTITUDE 400 //m
#define TRIGGER_THRESHOLD 30 //m
Adafruit_BMP280 bmp;

// Holds the value of the pressure for the zero altitude ( in hPa )
float initPressure, alt;
// Tell if we passed once the trigger altitude
boolean hasPassedTriggerAlt;
// Tell if we already triggered
boolean hasTriggered;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial ready, starting BMP...");

  if(!bmp.begin()){
    Serial.println("Couldn't find the BMP, check wiring.");
    while(1) delay(1000);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  initPressure = 0;
  getInitPressure();
  hasPassedTriggerAlt = false;
  alt = 0;

  Serial.printf("BMP ready. Altitude 0 pressure : %lf hPa\n", initPressure);
}

void loop() { 
  alt = bmp.readAltitude(initPressure);
  
  if((alt > TRIGGER_ALTITUDE + TRIGGER_THRESHOLD) && !hasPassedTriggerAlt && !hasTriggered){
    hasPassedTriggerAlt = true;
  } else if((alt <= TRIGGER_ALTITUDE) && hasPassedTriggerAlt && !hasTriggered) {
    Serial.printf("2nd event trigger now! Altitude : %lf\n", alt);
    trigger();
    hasTriggered = true;
  } else if(hasTriggered){
    while(1) delay(1000); // lock in infinite loop when done triggering
  }
  
}

void getInitPressure(){
  initPressure = 0;
  for(int i = 0; i<INIT_SAMPLES; i++){
    initPressure = initPressure + (bmp.readPressure() / 100); // bmp return the pressure in Pa and we want the initPressure in hPa
  }

  initPressure = initPressure / INIT_SAMPLES;
}

void triggerRE(){
  digitalWrite(TRIGGER_PIN, HIGH);
  delay(BURN_TIME);
  digitalWrite(TRIGGER_PIN, LOW);
}
