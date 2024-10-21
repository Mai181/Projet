/*
    Fichier : main.cpp
    Auteurs : P-19
    
    Date: 17 octobre 2024
    Description: Trouve les objets dans une arène 
                et va les porter dans les zones de 
                couleur
 */

#include <Arduino.h>
#include <librobus.h>

/********** 
 * Début de la zone des variables et constantes */
//


/**Délai en ms entre chaque itération du loop */
const int DT=50;
/** Boucle de débug */
const bool DEBUGAGE=false;


/********** FIN de la zone des variables et constantes
 * Début de la zone des fonctions */
//


/********** FIN de la zone des fonctions
 * Début du main */
//

/** Fonction de départ, se fait appeler une seule fois au début du programme*/
void setup(){
	BoardInit();
    Serial.begin(9600); //Communication à 9600 bits/sec
}

/** Fonction de départ, se fait appeler à chaque fois qu'elle est terminée */
void loop(){
    //boucle de test : code temporaire qui peut être remplacé et effacé
    while(DEBUGAGE){
        //code temporaire qui peut être remplacé et effacé
    }
    //fin boucle de test



    delay(DT);
}