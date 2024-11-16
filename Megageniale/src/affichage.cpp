/*    
*   Fichier: affichage.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions d'affichage
*/

#include "header.h"

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
    return true;
}
