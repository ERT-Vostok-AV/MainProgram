#include "buzzer.h"

const int buzPin = 23;  // A9
const int EMPin = 10;   // A8
const int burnTime = 500;
const int led = 13;

int lastTime, currTime, cnt = 0;

Buzzer buzz(buzPin);

void setup() {
  pinMode(EMPin, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
}

void loop() {
  if(millis()-lastTime >= 1000){
    cnt += 1;
    if(cnt >= 5){
      digitalWrite(led, HIGH);
      //buzz.initSuccess();
      digitalWrite(EMPin, HIGH);
      delay(burnTime);
      digitalWrite(EMPin, LOW);
      delay(500);
      digitalWrite(led, LOW);
      //buzz.initStart();
      while(1);
    } else {
      buzz.error();
      lastTime = millis();
    }
  }
}
