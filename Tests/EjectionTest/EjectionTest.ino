#include "buzzer.h"

const int buzPin = 14;
const int EMPin = 15;
const int burnTime = 200;

Buzzer buzzEclair(buzPin);

void setup() {
    pinMode(EMPin, OUTPUT);

    buzzEclair.error();
    delay(5000); //wait 5 sec before sending a current
    
    buzzEclair.initStart();
    digitalWrite(EMPin, HIGH);
    delay(burnTime);
    digitalWrite(EMPin, LOW);
}

void loop() {
    delay(1000);
}
