#include "buzzer.h"

const int led = 13;
const int buzPin = 14;
const int EMPin = 10 ;
const int burnTime = 2000;

//Buzzer buzzEclair(buzPin);

void setup() {
    pinMode(EMPin, OUTPUT);
    pinMode(led, OUTPUT);

    //buzzEclair.error();
    digitalWrite(led, HIGH);
    delay(1000);
    digitalWrite(led, LOW);
    
    delay(5000); //wait 5 sec before sending a current
    
    //buzzEclair.initStart();
    digitalWrite(EMPin, HIGH);
    digitalWrite(led, HIGH);
    delay(burnTime);
    digitalWrite(EMPin, LOW);
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
