/*    
*   Fichier: decisions.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de décisions
*/

#include "header.h"
#include "LibRobus.h"

const int SERVO_FERME = 15; //à redéterminer
int distributeurTempsAction=0;

// Variables globales
const float distanceDecalage = 40.0;  // À reprendre avec le distributeur<
int carte[20][20]; 

/**
* Fonction distributeur d'objet
* @return true si l'objet est déposé
*/
bool distributeur(bool actif){
    if(distributeurTempsAction>0)
    SERVO_SetAngle(1, SERVO_FERME);
    return true;
}

/**
* Fonction repérage dans l'espace
* @param action: (int) action effectuée par le robot
* @return la distance ou l'angle parcouru
*/
float reperage(int action){
    return 0;
}

/**
* Fonction écris la carte  
* @param detection: (int) ce que le robot a détecter
*/
void memoireCarte(int detection){

}
