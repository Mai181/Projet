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
#define PIN_RESET 46
#define PIN_BOUTON_1_CHANGE_GAUCHE 38
#define PIN_BOUTON_2_CHANGE_DROITE 40
#define PIN_BOUTON_3_SELECT 39
#define PIN_BOUTON_4_RETOUR 41
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
const float roueCirconference = 2.0*PI*(roueDiametre/2.0);
const float rouePulseCirconference = 3200.0;
const float roueDistance = 18.6;
const float rotationCirconference = 2.0*PI*(roueDistance);
const float pulse = (rotationCirconference)/((TOUR/ANGLE)*roueCirconference)*rouePulseCirconference;
const float vitesseGauche = 0.15;
const float vitesseDroite = 0.15;
const float pulseCm = rouePulseCirconference/roueDiametre;

const int CARTE_NB_LIGNES_MAX = 100;
const int CARTE_NB_COLONNES_MAX = 100;

struct Boutons
{
    bool change_gauche=0;
    bool change_droite=0;
    bool select=0;
    bool retour=0;
};

/**
* Fonction mise à jour des boutons sélectionnés 
*/
void boutonsUpdate();

/**
* Fonction retourne l'état des boutons
* @return une structure avec l'état de tous les boutons mis à jour
*/
struct Boutons boutonsGet();

/** 
* Fonction afficheur LCD
* @param clearBefore : (bool) Efface l'écritue préalablement si true
* @param texte : (String) texte à afficher 
* @return true si c'est exécutable
*/
bool affichageLCD(bool clearBefore, String texte);

/**
 * Fonction créant un menu navigant
 * @return Si changé de menu
 */
bool menu();

/**
 * Fonction créant un menu affichant la fin de tache
 * @return Si changé de menu
 */
bool menu_terminer();

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
 * Fonction reset encodeur
 */
void resetEncodeur();

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
* Fonction pour reset la carte
* @param x: (int) nombre de colonnes, déplacement horizontale
* @param y: (int) nombre de lignes, déplacement vertical
*/
void resetCarte(int x, int y);

/**
 * Fonction d'arbre de décision
 * @return true si exécuté correctement
 */
bool arbreDecision();

/** Donne la distance avec l'obstacle/objet devant, plus grande 
 * précision à 10cm suivi d'une imprécision grandissante en augmentant 
 * la distance et énormément grandissante en réduisant la distance
 * 
 * @return Une valeur réelle correspondant à la distance entre le 
 * capteur et l'objet
*/
float distanceObjet();

/**
 * Fonction pour attribuer la mise en marche du robot
 * @param enCoursTemp: (bool)
 */
void enCoursSet(bool enCoursTemp);

/**
 * Fonction pour avoir le nombre de lignes de la carte
 */
int yGet();

/**
 * Fonction pour avoir le nombre de colonnes de la carte
 */
int xGet();

/**
 * Fonction pour avoir la sensibilité
 */
int detectionSensibiliteGet();

/**
 * Fonction pour attribué la sensibilité
 * @param sensibiliteTemp: (int) 
 */
void detectionSensibiliteSet(int sensibiliteTemp);
