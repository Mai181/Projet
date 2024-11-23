/*    
*   Fichier: decisions.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de décisions
*/

#include "header.h"
#include "LibRobus.h"

const int SERVO_FERME = 52; //à redéterminer
const int SERVO_OUVERT = 4; //à redéterminer
int distributeurTempsAction=0;

bool enCours = false;
int menuSelection = 0;
struct Boutons boutons_decisions;

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
        distributeurTempsAction=11000; 
    if(distributeurTempsAction>0)
    {
    Serial.println("servo distributeur ouvert");
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_OUVERT);//pin à redéterminer
        distributeurTempsAction-=DELAIS;
    }else
    {
    Serial.println("servo distributeur fermer");
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_FERME);//pin à redéterminer
    }
    return true;
}

/** Fonction initialise servomoteurs
*/
void INIT_servos(){
    Serial.println("servo initialisation");
    SERVO_Enable(PIN_SERVO_DISTRIBUTEUR);
    SERVO_Enable(1);
    delay(100);
    SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_FERME);
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
    menu();
    boutons_decisions=boutonsGet();
    if(true)//enCours)
    {
        ajustementVitesse();
        if(detectionMetaux()){
            distributeur(true);
            allumerDEL(METAL, true);
        }

        else{
            
            distributeur(false);
            allumerDEL(METAL, false);
        }
        /*
        if(boutons_decisions.select && menuSelection == 0)
        {
                affichageLCD(MENU_INI_Y);
                enCours = 0;
        }else if(boutons_decisions.change && menuSelection == 0)
        {
                affichageLCD(MENU_INI_Y);
                menuSelection = 0;
        }
        */
        
    }else 
    {
        /*
        if(boutons_decisions.select && menuSelection == 0)
        {
                affichageLCD(MENU_INI_RECHERCHE_ARRETER);
                enCours = 1;
        }else if(boutons_decisions.select && menuSelection == 1)
        {
                affichageLCD(MENU_INI_N);
                enCours = 0;
        }else if(boutons_decisions.change && menuSelection == 0)
        {
                affichageLCD(MENU_INI_N);
                menuSelection = 1;
        }else if(boutons_decisions.change && menuSelection == 1)
        {
                affichageLCD(MENU_INI_Y);
                menuSelection = 0;
        }
        */
    }
    return true;
}

void enCoursSet(bool enCoursTemp)
{
    enCours=enCoursTemp;
}