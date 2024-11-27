/*    
*   Fichier: decisions.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de décisions
*/

#include "header.h"
#include "LibRobus.h"

const int SERVO_FERME = 10; //à redéterminer
const int SERVO_OUVERT = 73; //à redéterminer
int distributeurTempsAction=0;
int distributeurTempsAccumule=0;
float distObjet=0;

bool enCours = false;
bool enTransition = true;
bool enAvant = true;
bool enRotation = false;
bool premiereLigne = true;
float distanceParcourue = 0.0;
float distanceAReduire = roueDistance;
int dimensionX = 200;
int dimensionY = 200;
struct Boutons boutons_decisions;

// Variables globales
const float distanceDecalage = 40.0;  // À reprendre avec le distributeur<
int carteNbLignes = 100; //y
int carteNbColonnes = 100; //x
int carte[CARTE_NB_LIGNES_MAX][CARTE_NB_COLONNES_MAX]; 
int directionRotation = 1; // 1=droite, 0=gauche
int rangeeParcourue = 0;

/**
* Fonction distributeur d'objet
* @param actif: (bool) met en action si égale à TRUE
* @return true si l'objet est déposé
*/
bool distributeur(bool actif){                           
    if(actif)
    {
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_OUVERT);
    }else
    {
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_FERME);
    }
    return true;
}

/** Fonction initialise servomoteurs
*/
void INIT_servos(){
    Serial.println("servo initialisation");
    SERVO_Enable(PIN_SERVO_DISTRIBUTEUR);
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
* @param detection: (int) ce que le robot a détecter (1)
*/
void memoireCarte(int detection){
    for (int i = 0; i < carteNbLignes; i++){
        for (int j = 0; j < carteNbColonnes; j++){
            if(detection) carte[i][j] = 1;
        }
    }
}

/**
* Fonction pour reset la carte<
* @param x: (int) déplacement horizontal, nb de colonne
* @param y: (int) déplacement vertical, nb de ligne
*/
void resetCarte(int x, int y){
    dimensionX = x;
    dimensionY = y;
    carteNbLignes = y/2;
    carteNbColonnes = x/2;
    carte[carteNbLignes][carteNbColonnes];
    for (int i = 0; i < carteNbLignes; i++){
        for (int j = 0; j < carteNbColonnes; j++){
            carte[i][j] = 0;
        }
    }
}

/**
 * Fonction d'arbre de décision
 * @return true si exécuté correctement
 */
bool arbreDecision(){
    boutons_decisions=boutonsGet();
    if(rangeeParcourue >= dimensionY - 2 * roueDistance) 
    {
        menu_terminer();
        enCours = false;
        allumerDEL(false, MARCHE);
        allumerDEL(true, TERMINER);
        arreter();
    }
    else
        menu();
    if(enCours)
    {
        allumerDEL(true, MARCHE);
        if(detectionMetaux()){
            distributeur(true);
            allumerDEL(METAL, true);
        }
        else
        {
            distributeur(false);
            allumerDEL(METAL, false);
        }

        if(enAvant)
        {
            if(premiereLigne)
                distanceAReduire = roueDistance + 5;
            else
                distanceAReduire = 2 * roueDistance + 10;

            distanceParcourue = ((ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/2)*roueCirconference/rouePulseCirconference;
            
            if(distanceParcourue<dimensionX - distanceAReduire)
                ajustementVitesse();
            else
            {
                premiereLigne = false;
                enAvant = false;
                enRotation = true;
                resetEncodeur();
                rangeeParcourue += roueDistance;
            }
        }
        if(enRotation)
        {
            if (directionRotation) {
                rotation(DROITE);
                directionRotation--;
            }
            else {
                rotation(GAUCHE);
                directionRotation++;
            }
            resetEncodeur();
            enAvant = true;
            enRotation = false;
            arreter();
            delay(DELAIS*20);
        }

        /*if (distanceObjet()<10.0){                                // Si fonctionne pas, monte 10.0 à 15.0
            arreter();
            allumerDEL(OBSTACLE, true);
        }
        */
        
    }else 
    {
        allumerDEL(false, MARCHE);
    }
    return true;
}

void enCoursSet(bool enCoursTemp)
{
    enCours=enCoursTemp;
}

int yGet()
{
    return carteNbLignes*2;
}

int xGet()
{
    return carteNbColonnes*2;
}