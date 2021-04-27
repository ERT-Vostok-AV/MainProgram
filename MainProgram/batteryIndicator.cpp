#include "batteryIndicator.h"
#include "Arduino.h"

/**Error codes:
 * 0 no error
 * 1 no connection on pin
 * 
 * 
 */

BatteryIndicator::BatteryIndicator(int pin) : Sensor(0x00), pin(pin){
  maxV = batteryMaxV * (r1 / (r1 + r2));
  maxBin = (1024 / 3.3) * maxV;
}

int BatteryIndicator::begin(){
  pinMode(pin, INPUT);
  if (analogRead(pin) == 0){
    return 1;
  }
  return 0;
}


void BatteryIndicator::measure() {
  batteryLevel = (100 / maxBin) * analogRead(pin);
}


//accesseur
double BatteryIndicator::getBatteryLevel() {
    return batteryLevel;
}
