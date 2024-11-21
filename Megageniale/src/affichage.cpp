/*    
*   Fichier: affichage.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions d'affichage
*/

#include "header.h"

// Variables
float tempAccumuleAffichage = 0;
float tempRequisAffichage = 1000;
bool rougeAllume = false;
bool orangeAllume = false;
bool verteAllume = false;


/** 
* Fonction afficheur LCD
* @param texte: texte à afficher (String)
* @return true si c'est exécutable
*/
bool affichageLCD(String texte){

    // Vide l'écran
    DISPLAY_Clear();

    // Vérifie taille message
    if (texte.length() > 38) return false;

    // Positionnement curseur
    DISPLAY_SetCursor(0, 0);

    // Écriture du message
    DISPLAY_Printf(texte);


    return true;
}

/**
* Fonction pour allumer DEL couleur (logique inversée)
* @param etat: (int) état du robot 
* @param actif: (bool) true si l'état est actif
* @return true si c'est exécutable
*/
bool allumerDEL(int etat, bool actif){
    if(etat == MARCHE && actif == true) {
        digitalWrite(PINVERT, LOW);
        verteAllume = true;
        return true;
    }else if(etat == MARCHE){
        digitalWrite(PINVERT, HIGH);
        verteAllume = false;
        return true;
    }
    if(etat == METAL && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!rougeAllume){
                digitalWrite(PINROUGE, LOW);
                rougeAllume = true;
            }
            else{
                digitalWrite(PINROUGE, HIGH);
                rougeAllume = false;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    } else if (etat == METAL){
        digitalWrite(PINROUGE, HIGH);
        rougeAllume = false;
        return true;
    }
    if (etat == TERMINER && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!verteAllume){
                digitalWrite(PINVERT, LOW);
                verteAllume = true;
            }
            else{
                digitalWrite(PINVERT, HIGH);
                verteAllume = false;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    }else if (etat == TERMINER){
        digitalWrite(PINVERT, HIGH);
        verteAllume = false;
        return true;
    }
    if (etat == OBSTACLE && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!orangeAllume){
                digitalWrite(PINORANGE, LOW);
                orangeAllume = true;
            }
            else{
                digitalWrite(PINORANGE, HIGH);
                orangeAllume = false;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    } else if (etat == OBSTACLE){
        digitalWrite(PINORANGE, HIGH);
        orangeAllume = false;
        return true;
    }
    else if (etat == RIEN) {
        digitalWrite(PINVERT, HIGH);
        digitalWrite(PINROUGE, HIGH);
        digitalWrite(PINORANGE, HIGH);
        return true;
    }
    return false;
}
