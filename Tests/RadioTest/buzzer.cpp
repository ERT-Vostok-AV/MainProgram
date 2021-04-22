#include "buzzer.h"
#include "Arduino.h"

Buzzer::Buzzer(int pin) : pin(pin){
    pinMode(pin, OUTPUT);
  }

void Buzzer::beacon(){
    // S
    bip(1000, 200);
    delay(200);
    bip(1000, 200);
    delay(200);
    bip(1000, 200);

    delay(600);

    // O
    bip(1000, 600);
    delay(200);
    bip(1000, 600);
    delay(200);
    bip(1000, 600);

    delay(600);

    // S
    bip(1000, 200);
    delay(200);
    bip(1000, 200);
    delay(200);
    bip(1000, 200);
    
    
    // bip bip bip biiiip biiiip biiiip bip bip bip (a.k.a SOS)
}

void Buzzer::error(){
    bip(3000, 500);
    // biiiip 
}


void Buzzer::initSuccess(){
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
    // bip bip
}

void Buzzer::initStart(){
    bip(2000, 200);
    // bip (boring)
}

void Buzzer::bip(int freq, int duration){
    noTone(pin);
    tone(pin, freq);
    delay(duration);
    noTone(pin);
}
