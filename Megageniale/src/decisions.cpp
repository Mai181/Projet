/*    
*   Fichier: decisions.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de décisions
*/

#include "header.h"
#include "LibRobus.h"

const int SERVO_FERME = 15; //à redéterminer
const int SERVO_OUVERT = 105; //à redéterminer
int distributeurTempsAction=0;

// Variables globales
const float distanceDecalage = 40.0;  // À reprendre avec le distributeur<
int carte[20][20]; 

/**
* Fonction distributeur d'objet
* @param actif: (bool) met en action si égale à TRUE
* @return true si l'objet est déposé
*/
bool distributeur(bool actif){
    if(actif)
        distributeurTempsAction=1500;
    if(distributeurTempsAction>0)
    {
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_OUVERT);//pin à redéterminer
        distributeurTempsAction-=DELAIS;
    }else
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_FERME);//pin à redéterminer
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


/**
 * Fonction d'arbre de décision
 * @return true si exécuté correctement
 */
bool arbreDecision(){
    if(detectionMetaux())
        distributeur(true);
    else
        distributeur(false);
    return true;
}