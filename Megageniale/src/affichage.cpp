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
    
    // Longueur initiale du texte
    int longueurTexte = texte.length();

    // Vide l'écran
    DISPLAY_Clear();

    // Vérifie taille message
    if (texte.length() > 32) return false;

    // Positionnement curseur
    DISPLAY_SetCursor(0, 0);

    // Écriture du message
    if(texte.length() > 16){
        char str[texte.length()];
        texte.toCharArray(str, texte.length());
        if(texte.indexOf(" ") > 16){
            DISPLAY_Printf(strtok(str, &str[16]));
            DISPLAY_SetCursor(1, 0);
            DISPLAY_Printf(texte.substring(17));
        }
        else {
            while (texte.length() > 0){
                char* tok = strtok(str, " ");
                if ((longueurTexte - texte.length()) > 16) {
                    DISPLAY_SetCursor(1, 0);
                }
                DISPLAY_Printf(tok);
                texte.substring(texte.indexOf(" ")+1);
                texte.toCharArray(str, texte.length());
            }
        }
    }
    else DISPLAY_Printf(texte);


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
