#include "bmp280.h"

Bmp280 bmp;

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Serial ready");
  bmp.begin();
  Serial.println("bmp ready");  
  delay(500);
}

void loop(){
  bmp.measure();

  Serial.println(bmp.getAlt());

  delay(200);
  
}
