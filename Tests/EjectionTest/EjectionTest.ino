#include "buzzer.h"

#define led 13
#define EMPIN A8
#define buzPin A9

#define burnTime 1000 // ms
#define timeout 15000 // ms

Buzzer buzzer(buzPin);

void setup() {
    pinMode(EMPIN, OUTPUT);
    pinMode(led, OUTPUT);

    buzzer.initSuccess();
    digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led, LOW);
    delay(1000);

    int cnt = 0;
    while(cnt < 10){
      flash(100);
      buzzer.error();
      delay(1000);
      cnt += 1;
    }

    digitalWrite(EMPIN, HIGH);
    delay(burnTime);
    digitalWrite(EMPIN, LOW);
    digitalWrite(led, HIGH);
    buzzer.beacon();

}

void flash(int duration){
  digitalWrite(led, HIGH);
  delay(duration);
  digitalWrite(led, LOW);
}

void loop() {
}
