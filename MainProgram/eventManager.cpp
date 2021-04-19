#include "eventManager.h"



const double LIFTOFFALTITUDE = 20;			// Altitude minimum pour détecter le décollage
const double LIFTOFFSPEED = 2;				// Vitesse minimum pour le détection du décollage.
const double APOGEESPEEDCHANGE = -1;		// Idée c'est d'être sûr que la vitesse horizontale est bien négatif, et donc que l'apogée est passé
const double APOGEEDIFFERENCEALTITUDE = 3;	// En gros on va comparer maxAlt et l'altitude actuel, si la différence est plus grande que cette valeur alors il detecte que la fusée redescend.  
const double TRIGGERFOREMATCHALTITUDE = 300;//
const double TOUCHDOWNMAXALTITUDE = 20;		// Si il n'est pas déscendu en dessous de cette altitude il ne peut pas déclarer Touuchdown (retourner true)
const double TOUCHDOWNMAXSPEED = 1/2;	
const double TOUCHDOWNMINSPEED = -1/2;		// Si la vitesse verticale n'est pas comprise entre ces deux valeurs il ne peut pas déclarer le Touchdown (retourner true)

EventManager::EventManager() : triggered(false) {}

bool EventManager::isLiftOff(double alt, double velZ){
	bool data1 = alt > LIFTOFFALTITUDE;
	bool data2 = velz > LIFTOFFSPEED;
	return data1 or data2;	// à voir ce que l'on préfère
}

bool EventManager::isApogee(double alt, double velZ){
	if (alt > maxAlt){
	maxAlt = alt;
	}
	bool data1 = velZ < APOGEESPEEDCHANGE;					// détecte vitesse horizontale négative
	bool data2 = (maxAlt-APOGEEDIFFERENCEALTITUDE) > alt;	// détecte altitude qui redescent
	return data1 or data2;
	}

bool EventManager::isReTrigger(double alt){
	return alt <= TRIGGERFOREMATCHALTITUDE; //SOME RANDOM ALTITUDE NEED TO CHECK
}

bool EventManager::isTouchDown(double alt, double velZ){
	bool data1 = alt < TOUCHDOWNMAXALTITUDE;								// verifie que c'est après la descente
	bool data2 = (velZ < TOUCHDOWNMAXSPEED) and (velz > TOUCHDOWNMINSPEED);
																			// éventuellement une troisième condition altitude stagnant??
	return (data2 and data1);
}

void EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}
