/*    
*   Fichier: affichage.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions d'affichage
*/

#include "header.h"

// Variables
const int delais = 20;
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
    if (texte.length() > 16) {
        /*for (int i = 0; i < 16; i++){
            DISPLAY_Printf(texte[i]);
        }
        void DISPLAY_NewLine();
        for (int j = 16; j < texte.length(); j++){
            DISPLAY_Printf(texte[j]);
        }
        */
       DISPLAY_Printf(texte);
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
    
    if(etat == 1 && actif == true) {
        digitalWrite(48, HIGH);
    }
    if (etat == 1 && actif == false){
        digitalWrite(48, LOW);
    }
    if(etat == 2 && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            tempAccumuleAffichage -= tempRequisAffichage;
            switch (allume){
                case 0:
                    digitalWrite(50, HIGH);
                    allume = 1;
                    break;
                case 1:
                    digitalWrite(50, LOW);
                    allume = 0;
                    break;
            }
        }
        else {
            tempAccumuleAffichage += delais;
        }
    }
    if (etat == 2 && actif == false){
        digitalWrite(50, LOW);
    }
    if (etat == 3 && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            tempAccumuleAffichage -= tempRequisAffichage;
            switch (allume){
                case 0:
                    digitalWrite(48, HIGH);
                    allume = 1;
                    break;
                case 1:
                    digitalWrite(48, LOW);
                    allume = 0;
                    break;
            }
        }
        else {
            tempAccumuleAffichage += delais;
        }
    }
    if (etat == 3 && actif == false){
        digitalWrite(48, LOW);
    }
    if (etat == 4 && actif == true) {
        if(tempAccumuleAffichage > tempRequisAffichage){
            tempAccumuleAffichage -= tempRequisAffichage;
            switch (allume){
                case 0:
                    digitalWrite(46, HIGH);
                    allume = 1;
                    break;
                case 1:
                    digitalWrite(46, LOW);
                    allume = 0;
                    break;
            }
        }
        else {
            tempAccumuleAffichage += delais;
        }
    }
    if (etat == 4 && actif == false){
        digitalWrite(46, LOW);
    }
    else{
        digitalWrite(50, LOW);
        digitalWrite(48, LOW);
        digitalWrite(46, LOW);
    }

    return true;
}
