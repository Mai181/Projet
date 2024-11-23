/*    
*   Fichier: affichage.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions d'affichage
*/

#include "header.h"

struct MenuVariables
{
    bool actif=0;
    int selection=1;
    int nbOption=0;
};
struct Boutons boutons;
int nbBoutonsEnfonce = 0;


/** Menu principal */
bool menu();
/** Variables de navigation du menu principal */
MenuVariables menu_variables;

/** Menu  */
bool menu_commencer();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_variables;

/** Menu  */
bool menu_commencer_arreter();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_arreter_variables;

/** Menu  */
bool menu_commencer_pause();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_variables;

/** Menu  */
bool menu_commencer_pause_reglage();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_reglage_variables;

/** Menu  */
bool menu_commencer_pause_arreter();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_arreter_variables;

/** Menu  */
bool menu_commencer_pause_reglage_sensibilite();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_reglage_sensibilite_variables;

/** Menu  */
bool menu_commencer_pause_reglage_mapReset();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_reglage_mapReset_variables;

/** Menu  */
bool menu_reglage();
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_variables;

/** Menu  */
bool menu_reglage_sensibilite();//menu_commencer_pause_reglage_sensibilite
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_sensibilite_variables;

/** Menu  */
bool menu_reglage_dimension();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_variables;

/** Menu  */
bool menu_reglage_mapReset();
/** Variables de navigation du menu_reglage_mapReset */
MenuVariables menu_reglage_mapReset_variables;


// Variables
float tempAccumuleAffichage = 0;
float tempRequisAffichage = 500;
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
    if (texte.length() > 38) return false;

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

void menuAffichage()
{

}

bool menu()
{
    boutons=boutonsGet();
    nbBoutonsEnfonce = 0;
    menu_variables.actif = 1;
    menu_variables.nbOption = 2;
    if(menu_commencer_variables.actif) menu_commencer();
    else if(menu_reglage_variables.actif) menu_reglage();
    else
    {
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1)
        {
            if(boutons.change_gauche)
            {
                affichageLCD("------------------------");
                menu_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_variables.selection++;
            }
            else if(boutons.select)
            {
                affichageLCD("///////////");
                if(menu_variables.selection==1) 
                {
                    avancer();
                    menu_commencer();
                }
                else if(menu_variables.selection==2) menu_reglage();
                return 1;
            }
            else if(boutons.retour)
            {
                menu_variables.selection = 1;
            }
            if(menu_variables.selection>menu_variables.nbOption) menu_variables.selection = 1;
            else if(menu_variables.selection<1) menu_variables.selection = menu_variables.nbOption;

            if(menu_variables.selection == 1)
            {
                affichageLCD("Menu principal  ////>Commencer  Regl ");
                //            1234567890123456/89/1234567890123456_
            }
            else if(menu_variables.selection == 2)
            {
                affichageLCD("Menu principal  ////mmencer >Reglage ");
                //            1234567890123456/89/1234567890123456_
            }
        }
    }

}

bool menu_commencer()
{

}

bool menu_commencer_arreter()
{

}

bool menu_commencer_pause()
{

}

bool menu_commencer_pause_reglage()
{
    menu_commencer_arreter();
}

bool menu_commencer_pause_arreter()
{
    
}

bool menu_commencer_pause_reglage_sensibilite()
{
    menu_reglage_sensibilite();
}
bool menu_commencer_pause_reglage_mapReset()
{
    menu_reglage_mapReset();
}

bool menu_reglage()
{

}

bool menu_reglage_sensibilite()//menu_commencer_pause_reglage_sensibilite
{

}

bool menu_reglage_dimension()//menu_commencer_pause_reglage_dimension
{

}

bool menu_reglage_mapReset()
{

}