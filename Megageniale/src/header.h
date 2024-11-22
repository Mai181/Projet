/*    
*   Fichier: header.h
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: header des fonctions du main 
*                (les fonctions sont implémentées 
*                dans des fichiers différents du main)
*/

#pragma once
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <LibRobus.h>

#define RIEN 0
#define MARCHE 1
#define METAL 2
#define TERMINER 3
#define OBSTACLE 4
#define DROITE 1
#define GAUCHE 0
#define RESET 0
#define AVANCER 1
#define TOURNER 2
#define DELAIS 20

#define PINROUGE 45
#define PINORANGE 44
#define PINVERT 47
#define PIN_BOUTON_1 39
#define PIN_BOUTON_2_CHANGE 38
#define PIN_BOUTON_3_SELECT 41
#define PIN_BOUTON_4 40
#define PIN_SERVO_DISTRIBUTEUR RIGHT
#define PIN_ANALOG_DETECTEUR_METAUX_1 A4
#define PIN_ANALOG_DETECTEUR_METAUX_2 A5
#define PIN_ANALOG_DETECTEUR_METAUX_3 A6


#define MENU_INI_Y "Commencer la    ////recherche: >Y  N "
#define MENU_INI_N "Commencer la    ////recherche:  Y >N "
#define MENU_INI_RECHERCHE_ARRETER "Arreter la      ////recherche: >Y"

#define ANGLE 180.0
#define TOUR 360.0

const float roueDiametre = 7.6;
const float roueCirconference = 2*PI*(roueDiametre/2);
const float rouePulseCirconference = 3200.0;
const float roueDistance = 18.6;
const float rotationCirconference = 2*PI*(roueDistance/2);
const float pulse = (rotationCirconference)/((TOUR/ANGLE)*roueCirconference)*rouePulseCirconference;
const float vitesseGauche = 0.1;
const float vitesseDroite = 0.1;

struct Boutons
{
    bool select=0;
    bool change=0;
};

/**
* Fonction mise à jour des boutons sélectionnés 
*/
void boutonsUpdate();

/**
* Fonction retourne l'état des boutons
*
* @return une structure avec l'état de tous les boutons mis à jour
*/
struct Boutons boutonsGet();

/**
* Fonction afficheur LCD
* @param texte: (String) message à afficher
* @return true si l'affichage est possible
*/
bool affichageLCD(String texte);

/**
* Fonction pour allumer DEL couleur
* @param etat: (int) état du robot 
* @param actif: (bool) true si l'état est actif
* @return true si c'est exécutable
*/
bool allumerDEL(int etat, bool actif);

/**
* Fonction détection métaux
* @return true si un métal est détecté
*/
bool detectionMetaux();

/**
* Fonction distributeur d'objet
* @param actif: (bool) met en action si égale à TRUE
* @return true si l'objet est déposé
*/
bool distributeur(bool actif);

/** Fonction initialise servomoteurs
*/
void INIT_servos();

/**
* Fonction avancement
*/
void avancer();

/**
* Fonction arret
*/
void arreter();

/** Fonction pour limiter une valeur dans une plage donnée
 *  @param valeur: valeur (float)
 *  @param minVal: son minimum (float)
 *  @param maxVal: son maximum (float)
 *  @return minVal si < valeur, maxVal si > valeur ou valeur 
*/
float limiter(float valeur, float minVal, float maxVal);

/**
* Fonction ajustement vitesse
*/
void ajustementVitesse();

/**
* Fonction rotation
* @param direction: (int) tourner à gauche ou à droite
*/
void rotation(int direction);

/**
* Fonction détection obstacle (facultatif)
* @param 
* @return 
*/


/**
* Fonction repérage dans l'espace
* @param action: (int) action effectuée par le robot
* @return la distance ou l'angle parcouru
*/
float reperage(int action);

/**
* Fonction écris la carte des positions des métaux
* @param detection: (int) ce que le robot a détecter
*/
void memoireCarte(int detection);

/**
 * Fonction d'arbre de décision
 * @return true si exécuté correctement
 */
bool arbreDecision();