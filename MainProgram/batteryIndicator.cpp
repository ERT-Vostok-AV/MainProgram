#include "batteryIndicator.h"
#include "Arduino.h"

/**Error codes:
 * 0 no error
 * 1 no connection on pin
 */

/*
 * @brief Constructor
 * @param pin : Pin from which the battery level is read
 */
BatteryIndicator::BatteryIndicator(int pin) : Sensor(0x00), pin(pin){
  maxV = batteryMaxV * (r1 / (r1 + r2));
  maxBin = (1024 / 3.3) * maxV;
}

/*
 * @brief initialize the battery indicator
 */
int BatteryIndicator::begin(){
  pinMode(pin, INPUT);
  if (analogRead(pin) == 0){
    return 1;
  }
  return 0;
}

/*
 * @brief measures the battery level and updated it's battery level attribute
 */
void BatteryIndicator::measure() {
  batteryLevel = (100 / maxBin) * analogRead(pin);
}


/*
 * @brief getter for the battery level
 */
double BatteryIndicator::getBatteryLevel() {
    return batteryLevel;
}
