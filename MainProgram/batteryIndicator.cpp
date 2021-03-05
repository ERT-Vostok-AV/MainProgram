#include "batteryIndicator.h"
#include "Arduino.h"

/**Error codes:
 * 0 no error
 * 1 no connection on pin
 * 
 * 
 */

BatteryIndicator::BatteryIndicator(int pinC) : Sensor(0x00){pin = pinC;}


void BatteryIndicator::measure() {
  batteryLevel = (100 / 921.5) * analogRead(pin);
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
