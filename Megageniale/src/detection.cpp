/*    
*   Fichier: detection.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de déplacement
*/

#include "header.h"

/**
* Fonction détection métaux
* @return true si un métal est détecté
*/
bool detectionMetaux(){
    if(analogRead(PIN_ANALOG_DETECTEUR_METAUX)>300)//à redéterminer la pin et tester la sensibilité (si 300 est trop élevé ou pas assez)
        return true;
    else
        return false;
}

