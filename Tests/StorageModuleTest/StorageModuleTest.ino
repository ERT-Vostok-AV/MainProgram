
#include "flightData.h"
#include "storage.h"

Storage sdcard;


void setup () {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Serial Ready. ");
  Serial.println("Initializing SD card...");
  sdcard.begin();
  
  Serial.println("initialization done.");
}

void loop() {
  
}
