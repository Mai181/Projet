/*    
*   Fichier: detection.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de déplacement
*/

#include "header.h"

const int detectionMin=600;
struct Boutons boutonsDetection;

/**
* Fonction détection métaux
* @return true si un métal est détecté
*/
bool detectionMetaux(){
    /*
    Serial.print("metaux 1 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_1));
    Serial.print("metaux 2 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_2));
    Serial.print("metaux 3 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_3));
    */
    if(analogRead(PIN_ANALOG_DETECTEUR_METAUX_1)>detectionMin || analogRead(PIN_ANALOG_DETECTEUR_METAUX_2)>detectionMin || analogRead(PIN_ANALOG_DETECTEUR_METAUX_3)>detectionMin)//à redéterminer la pin et tester la sensibilité (si 300 est trop élevé ou pas assez)
        return true;
    else
        return false;
}


/**
* Fonction mise à jour des boutons sélectionnés 
*/
void boutonsUpdate(){
    boutonsDetection.change_gauche=digitalRead(PIN_BOUTON_1_CHANGE_GAUCHE);
    boutonsDetection.change_droite=digitalRead(PIN_BOUTON_2_CHANGE_DROITE);
    boutonsDetection.select=digitalRead(PIN_BOUTON_3_SELECT);
    boutonsDetection.retour=digitalRead(PIN_BOUTON_4_RETOUR);
}

/**
* Fonction retourne l'état des boutons
*
* @return une structure avec l'état de tous les boutons mis à jour
*/
struct Boutons boutonsGet(){
    return boutonsDetection;
}