#include "batteryIndicator.h"
#include "Arduino.h"

BatteryIndicator::BatteryIndicator(int pinC) : Sensor(0x00){pin = pinC;}


void BatteryIndicator::measure() {
  batteryLevel = (100 / 921.5) * analogRead(pin);
}

bool BatteryIndicator::begin(){
  pinMode(pin, INPUT);
  return true;
}

//accesseur
double BatteryIndicator::getBatteryLevel() {
    return batteryLevel;
}
