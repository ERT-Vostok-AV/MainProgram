#include "bmp280.h"

#define BMP_PIN A7 //TODO
#define E_MATCH_PIN

bmp280 bmp(BMP_PIN)

void setup() {
  bmp.begin()
}

void loop() {
  // put your main code here, to run repeatedly:

}
