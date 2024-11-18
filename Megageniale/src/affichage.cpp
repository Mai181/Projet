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
bool allume = true;


/** 
* Fonction afficheur LCD
* @param texte: texte à afficher (String)
* @return true si c'est exécutable
*/
bool affichageLCD(String texte){
    
    // Vide l'écran
    DISPLAY_Clear();

    // Vérifie taille message
    if (texte.length() > 32) return false;

    // Positionnement curseur
    DISPLAY_SetCursor(0, 0);

    // Écriture du message
    DISPLAY_Printf(texte);

    return true;
}

/**
* Fonction pour allumer DEL couleur
* @param etat: (int) état du robot 
* @param actif: (bool) true si l'état est actif
* @return true si c'est exécutable
*/
bool allumerDEL(int etat, bool actif){
    if(etat == MARCHE && actif == true) {
        digitalWrite(PINVERT, HIGH);
        return true;
    }else if(etat == MARCHE){
        digitalWrite(PINVERT, LOW);
        return true;
    }
    if(etat == METAL && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!allume){
                digitalWrite(PINROUGE, HIGH);
                allume = 1;
            }
            else{
                digitalWrite(PINROUGE, LOW);
                allume = 0;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    } else if (etat == METAL){
        digitalWrite(PINROUGE, LOW);
        return true;
    }
    if (etat == TERMINER && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!allume){
                digitalWrite(PINVERT, HIGH);
                allume = 1;
            }
            else{
                digitalWrite(PINVERT, LOW);
                allume = 0;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    }else if (etat == TERMINER){
        digitalWrite(PINVERT, LOW);
        return true;
    }
    if (etat == OBSTACLE && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            if(!allume){
                digitalWrite(PINORANGE, HIGH);
                allume = 1;
            }
            else{
                digitalWrite(PINORANGE, LOW);
                allume = 0;
            }
            tempAccumuleAffichage -= tempRequisAffichage;
        }
        else {
            tempAccumuleAffichage += DELAIS;
        }
        return true;
    } else if (etat == OBSTACLE){
        digitalWrite(PINORANGE, LOW);
        return true;
    }
    else if (etat == RIEN) {
        digitalWrite(PINVERT, LOW);
        digitalWrite(PINROUGE, LOW);
        digitalWrite(PINORANGE, LOW);
        return true;
    }
    return false;
}
