#include "buzzer.h"

const int buzPin = 23;  // A9
const int EMPin = 22;   // A8
const int burnTime = 500;

int lastTime, currTime, cnt = 0;

Buzzer buzz(buzPin);

void setup() {
  pinMode(EMPin, OUTPUT);
}

void loop() {
  if(millis()-lastTime >= 1000){
    cnt += 1;
    if(cnt >= 10){
      buzz.initSuccess();
      digitalWrite(EMPin, HIGH);
      delay(burnTime);
      digitalWrite(EMPin, LOW);
      delay(500);
      buzz.initStart();
      while(1);
    } else {
      buzz.error();
      lastTime = millis();
    }
  }
}
