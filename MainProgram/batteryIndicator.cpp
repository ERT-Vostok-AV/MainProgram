#include "batteryIndicator.h"
#include "Arduino.h"

/**Error codes:
 * 0 no error
 * 1 no connection on pin
 * 
 * 
 */

BatteryIndicator::BatteryIndicator(int pinC) : Sensor(0x00){
  pin = pinC; 
  maxV = 12 * (r1 / (r1 + r2));
  maxBin = (1024 / 3.3) * maxV;
}


void BatteryIndicator::measure() {
  batteryLevel = (100 / maxBin) * analogRead(pin);
}

int BatteryIndicator::begin(){
  pinMode(pin, INPUT);
  if (analogRead(pin) == 0){
    return 1;
  }
  return 0;
}

//accesseur
double BatteryIndicator::getBatteryLevel() {
    return batteryLevel;
}
