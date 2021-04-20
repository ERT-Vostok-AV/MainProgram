#include "buzzer.h"
#include "Arduino.h"

/*
 * Constructor: initialize the battery pin as an output
 */
Buzzer::Buzzer(int pin) : pin(pin){
    pinMode(pin, OUTPUT);
  }

/*
 * Produce a beacon sound as 
 * - - - . . . - - - (SOS)
 */
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
}

/*
 * Produces an error sound as :
 * Half a second high pitch note
 */
void Buzzer::error(){
    bip(3000, 500);
    // biiiip 
}

/*
 * Produces a success sound after initialisation as :
 * Two bips lasting a 10th of a second seperated by 
 * a 10th of a seconds
 */
void Buzzer::initSuccess(){
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
    // bip bip
}

void Buzzer::reTrigger(){
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
}

void Buzzer::initStart(){
    bip(2000, 200);
    // bip (boring)
}

void Buzzer::liftoff(){
    bip(3000, 500);
    delay(300);
    bip(3000, 500);
}

void Buzzer::apogee(){ // _ . _
    bip(3000, 500);
    delay(300);
    bip(2000, 200);
    delay(300);
    bip(3000, 500);
}

void Buzzer::bip(int freq, int duration){
    noTone(pin);
    tone(pin, freq);
    delay(duration);
    noTone(pin);
}
