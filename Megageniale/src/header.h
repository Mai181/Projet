/*    
*   Fichier: header.h
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: header des fonctions du main 
*                (les fonctions sont implémentées 
*                dans des fichiers différents du main)
*/

#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <Librobus.h>

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

// Variables
const int delais = 20;
float tempAccumuleAffichage = 0;
float tempRequisAffichage = 1000;
bool allume = true;

/**
* Fonction bouton sélectionné (à déterminer)
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
* @return true si l'objet est déposé
*/
bool distributeur();

/**
* Fonction avancement
*/
void avancer();

/**
* Fonction arret
*/
void arreter();

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
