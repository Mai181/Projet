/*    
*   Fichier: decisions.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de décisions
*/

#include "header.h"
#include "LibRobus.h"

const int SERVO_FERME = 73; //à redéterminer
const int SERVO_OUVERT = 10; //à redéterminer
int distributeurTempsAction=0;
int distributeurTempsAccumule=0;
float distObjet = 50*rouePulseCirconference/roueCirconference;
float positionObjet = 0.0;
bool distribution = false;

bool enCours = false;
bool enTransition = true;
bool enAvant = true;
bool enRotation = false;
bool enArret = false;
bool premiereLigne = true;
bool premiereFin = true;
float accelerationTempsAction = 300;
float accelerationTemps = accelerationTempsAction;
int accelerationPhase = 1;
float distanceParcourue = 0.0;
float distanceAReduire = roueDistance;
int dimensionX = 200;
int dimensionY = 100;
struct Boutons boutons_decisions;

// Variables globales
const float distanceDecalage = 40.0;  // À reprendre avec le distributeur<
int carteNbLignes = 50; //y
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
        distribution = true;
        distributeurTempsAction=10000;
        positionObjet = (ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/-2.0;
    }
    if(distribution)
    {
        distributeurTempsAction-=DELAIS;
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_OUVERT);
        if(distObjet < (ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/-2.0 - positionObjet || distributeurTempsAction < 0)
        {
            distribution = false;
        }
    }
    else
    {
        SERVO_SetAngle(PIN_SERVO_DISTRIBUTEUR, SERVO_FERME);
        distributeurTempsAction = 0;
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
    if(rangeeParcourue >= dimensionY - 1 * roueDistance) 
    {
        if(premiereFin)
        {
            premiereFin = false;
        }
            allumerDEL(MARCHE, false);
        menu_terminer();
        enCours = false;
        allumerDEL(TERMINER, true);
        arreter();
    }
    else
        menu();
    if(enCours)
    {
        allumerDEL(true, MARCHE);

        if(enAvant)
        {
            if(distanceObjet()<=35.0)             // Si fonctionne pas, monte 10.0 à 15.0
            {                   
                arreter();
                allumerDEL(OBSTACLE, true);
                enArret = true;
            }
            else
            {
                allumerDEL(OBSTACLE, false);
                enArret = false;

                if(premiereLigne)
                    distanceAReduire = roueDistance + 5;
                else
                    distanceAReduire = 2 * roueDistance + 10;

                distanceParcourue = (-(ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/2)*roueCirconference/rouePulseCirconference;
                
                if(distanceParcourue<dimensionX - distanceAReduire)
                {
                    ajustementVitesse(1);
                }
                else
                {
                    premiereLigne = false;
                    enAvant = false;
                    enRotation = true;
                    resetEncodeur();
                    rangeeParcourue += roueDistance;
                    arreter();
                    delay(750);
                }
                
                if(detectionMetaux()){
                    distributeur(true);
                    allumerDEL(METAL, true);
                }
                else
                {
                    distributeur(false);
                    allumerDEL(METAL, false);
                }
            }
        }
        else if(enRotation)
        {
            allumerDEL(OBSTACLE, false);

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
            enTransition = true;
            accelerationPhase = 0;
            accelerationTemps = accelerationTempsAction;
            arreter();
            delay(750);
        }
        
    }else 
    {
        allumerDEL(false, MARCHE);
    }


    return true;
}

void enCoursSet(bool enCoursTemp)
{
    enCours=enCoursTemp;
    accelerationTemps = accelerationTempsAction;
    accelerationPhase = 0;
    enTransition = true;
    if(enCours)
        allumerDEL(MARCHE, true);
    else   
        allumerDEL(MARCHE, false);
}

int yGet()
{
    return dimensionY;
}

int xGet()
{
    return dimensionX;
}