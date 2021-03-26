
#include "flightData.h"
#include "storage.h"
#include <Vector.h>

#define BUFFER_ELEMENT_COUNT_MAX 100

Storage sdcard;


typedef Vector<FlightData> Buffer;
FlightData bufferStorageArray[BUFFER_ELEMENT_COUNT_MAX];

Buffer buffer(bufferStorageArray);

FlightData measures;

int curr = 0;

int n = 10; //number of data in the dummy buffer

void setup () {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Serial Ready. ");
  Serial.println("Initializing SD card...");
  sdcard.begin();
  Serial.println("initialization done. Creating dummy buffer...");
  sdcard.initLog();

  for (int i = 0; i < n; i++)
  {
    measures.batteryLevel = i;
    measures.altitude = i;
    measures.temperature = i;
    measures.velocity[0] = i;
    measures.velocity[1] = i;
    measures.velocity[2] = i;
    measures.rotation[0] = i;
    measures.rotation[1] = i;
    measures.rotation[2] = i;
    buffer.push_back(measures);
  }

  Serial.println("Saving it on SD card");

//  long duration = micros();
//  sdcard.saveSD(buffer);
//  duration = micros() - duration;
//  sdcard.logFlightInfo(0,1,2,3);
//  Serial.println("Saving finished !");
//  Serial.print("It took "); Serial.print(duration); Serial.print("µs to save "); Serial.print(n); Serial.println(" measures");
  curr = millis();
}


void loop() {
  sdcard.saveSD(buffer);
  curr = millis();
  if(curr > 10000){
    sdcard.logFlightInfo(0,0,0,0);
    Serial.println("FINI");
    while(1);
  } 
  delay(10);
}
