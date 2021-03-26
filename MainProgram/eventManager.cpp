#include "eventManager.h"



const double LIFTOFFALTITUDE = 20;			// Altitude minimum pour d�tecter le d�collage
const double LIFTOFFSPEED = 2;				// Vitesse minimum pour le d�tection du d�collage.
const double APOGEESPEEDCHANGE = -1;		// Id�e c'est d'�tre s�r que la vitesse horizontale est bien n�gatif, et donc que l'apog�e est pass�
const double APOGEEDIFFERENCEALTITUDE = 3;	// En gros on va comparer maxAlt et l'altitude actuel, si la diff�rence est plus grande que cette valeur alors il detecte que la fus�e redescend.  
const double TRIGGERFOREMATCHALTITUDE = 300;//
const double TOUCHDOWNMAXALTITUDE = 20;		// Si il n'est pas d�scendu en dessous de cette altitude il ne peut pas d�clarer Touuchdown (retourner true)
const double TOUCHDOWNMAXSPEED = 1/2;	
const double TOUCHDOWNMINSPEED = -1/2;		// Si la vitesse verticale n'est pas comprise entre ces deux valeurs il ne peut pas d�clarer le Touchdown (retourner true)

EventManager::EventManager() : triggered(false) {}

bool EventManager::isLiftOff(double alt, double velZ){
	bool data1 = alt > LIFTOFFALTITUDE;
	bool data2 = velZ > LIFTOFFSPEED;
	return data1 or data2;	// � voir ce que l'on pr�f�re
}

bool EventManager::isApogee(double alt, double velZ){
	if (alt > maxAlt){
	maxAlt = alt;
	}
	bool data1 = velZ < APOGEESPEEDCHANGE;					// d�tecte vitesse horizontale n�gative
	bool data2 = (maxAlt-APOGEEDIFFERENCEALTITUDE) > alt;	// d�tecte altitude qui redescent
	return data1 or data2;
	}

bool EventManager::isReTrigger(double alt){
	return alt <= TRIGGERFOREMATCHALTITUDE; //SOME RANDOM ALTITUDE NEED TO CHECK
}

bool EventManager::isTouchDown(double alt, double velZ){
	bool data1 = alt < TOUCHDOWNMAXALTITUDE;								// verifie que c'est apr�s la descente
	bool data2 = (velZ < TOUCHDOWNMAXSPEED) and (velZ > TOUCHDOWNMINSPEED);
																			// �ventuellement une troisi�me condition altitude stagnant??
	return (data2 and data1);
}

void EventManager::trigger(){
    //TODO allume l'e-match
    triggered = true;
}

bool EventManager::hasTriggered(){
    return triggered;
}
