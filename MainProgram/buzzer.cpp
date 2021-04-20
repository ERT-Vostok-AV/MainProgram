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

/*
 * Prodcues a bip when the 2nd event has been triggered as:
 * 3 bip of a 10th of a second separated by a 10th of a sec
 */
void Buzzer::reTrigger(){
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
    delay(100);
    bip(2000, 100);
}

/*
 * Porduces a simple bip of a fifth of a second
 */
void Buzzer::initStart(){
    bip(2000, 200);
    // bip (boring)
}

/*
 * Produces two bips of half a second 
 */
void Buzzer::liftoff(){
    bip(3000, 500);
    delay(300);
    bip(3000, 500);
}

/*
 * Produces two long bips separated by a short one
 */
void Buzzer::apogee(){ // _ . _
    bip(3000, 500);
    delay(300);
    bip(2000, 200);
    delay(300);
    bip(3000, 500);
}

/*
 * Given a frequence and a duration, makes the buzzer bip
 * @param freq : frequence of the bip
 * @parm duration : duration of the bip
 */
void Buzzer::bip(int freq, int duration){
    noTone(pin);
    tone(pin, freq);
    delay(duration);
    noTone(pin);
}
