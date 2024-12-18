/*    
*   Fichier: affichage.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions d'affichage
*/

#include "header.h"

// Variables
struct MenuVariables
{
    bool actif=0;
    int selection=1;
    int nbOption=0;
};
struct Boutons boutons;
int nbBoutonsEnfonce = 0;
bool menuUpdate = true;
char str[37]; /** String temporaire*/

const int DIMENSION_X_MIN = 100;
const int DIMENSION_X_MAX = 200;             //limite du champ à analyzer
const int DIMENSION_Y_MIN = 30;
const int DIMENSION_Y_MAX = 180;
int dimensionXModifie = 100;
int dimensionYModifie = 100;

const int SENSIBILITE_MIN = 500;
const int SENSIBILITE_MAX = 1000;
int sensibiliteModifie = 600;

int tempAccumuleAffichage = 0;
int tempRequisAffichage = 1000;
int menuTempsAccumule = 0;
int menuTempsRequis = 1000;
int menuTerminerTempsAccumule = 0;
int menuTerminerTempsRequis = 1000;
bool rougeAllume = false;
bool orangeAllume = false;
bool verteAllume = false;

// Header Menus
/** Menu principal */
bool menu();
/** Variables de navigation du menu principal */
MenuVariables menu_variables;

/** Menu commencer */
bool menu_commencer();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_variables;

/** Menu commencer pause */
bool menu_commencer_pause();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_variables;

/** Menu commencer pause réglage */
bool menu_commencer_pause_reglage();
/** Variables de navigation du menu principal */
MenuVariables menu_commencer_pause_reglage_variables;

/** Menu réglage */
bool menu_reglage();
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_variables;

/** Menu réglage sensibilité */
bool menu_reglage_sensibilite();//menu_commencer_pause_reglage_sensibilite
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_sensibilite_variables;

/** Menu réglage dimension */
bool menu_reglage_dimension();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_variables;

/** Menu réglage dimension x */
bool menu_reglage_dimension_x();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_x_variables;

/** Menu réglage dimension y */
bool menu_reglage_dimension_y();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_y_variables;

/** Menu réglage mapReset */
bool menu_reglage_mapReset();
/** Variables de navigation du menu_reglage_mapReset */
MenuVariables menu_reglage_mapReset_variables;

/** Menu réglage mapReset confirmation */
bool menu_reglage_mapReset_confirmation();
/** Variables de navigation du menu_reglage_mapReset */
MenuVariables menu_reglage_mapReset_confirmation_variables;

/** Menu réglage sensibilité confirmation */
bool menu_reglage_sensibilite_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_sensibilite_confirmation_variables;

/** Menu réglage dimension x confirmation */
bool menu_reglage_dimension_x_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_dimension_x_confirmation_variables;

/** Menu réglage dimension y confirmation */
bool menu_reglage_dimension_y_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_dimension_y_confirmation_variables;

/** Menu de fin de tache */
bool menu_terminer();
/** Variables de navigation du menu_terminer */
MenuVariables menu_terminer_variables;
bool premierTerminer = true;

/** 
* Fonction afficheur LCD
* @param clearBefore : (bool) Efface l'écriture préalablement si true
* @param texte : (String) texte à afficher 
* @return true si c'est exécutable
*/
bool affichageLCD(bool clearBefore, String texte){

    // Vide l'écran
    if(clearBefore) DISPLAY_Clear();

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
        digitalWrite(PINROUGE, LOW);
        rougeAllume = true;
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
                tempAccumuleAffichage -= DELAIS/1.5;
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
        digitalWrite(PINORANGE, LOW);
        orangeAllume = true;
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

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_variables.selection==1) //Commencer
                {
                    enCoursSet(true);
                    ajustementVitesse(1);
                    menu_commencer();
                }
                else if(menu_variables.selection==2) menu_reglage(); //Réglage
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
                affichageLCD(true, "Menu principal  ////>Commencer Regla ");
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_variables.selection == 2)
            {
                affichageLCD(true, "Menu principal  ////mmencer >Reglage ");
                //                  1234567890123456/89/1234567890123456_
            }
        }
    }
    return 0;
}

bool menu_commencer()
{
    nbBoutonsEnfonce = 0;
    menu_commencer_variables.actif = 1;
    menu_commencer_variables.nbOption = 1;

    if(menu_commencer_pause_variables.actif) menu_commencer_pause();
    else
    {
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_commencer_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_commencer_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_commencer_variables.selection==1) //Pause
                {
                    enCoursSet(false);
                    arreter();
                    menu_commencer_pause();
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menuUpdate = true;
                boutons.retour = false;
                menu_commencer_variables.selection = 1;
            }
            if(menu_commencer_variables.selection>menu_commencer_variables.nbOption) menu_commencer_variables.selection = 1;
            else if(menu_commencer_variables.selection<1) menu_commencer_variables.selection = menu_commencer_variables.nbOption;

            if(menu_commencer_variables.selection == 1)
            {
                affichageLCD(true, "Menu            ////En cours: >Pause ");
                //                  1234567890123456/89/1234567890123456_
            }
        }
    }
    return 0;
}

bool menu_commencer_pause()
{
    nbBoutonsEnfonce = 0;
    menu_commencer_pause_variables.actif = 1;
    menu_commencer_pause_variables.nbOption = 2;
    if(menu_commencer_pause_reglage_variables.actif) menu_commencer_pause_reglage();
    else
    {
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_commencer_pause_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_commencer_pause_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_commencer_pause_variables.selection==1) //Continuer
                {
                    ajustementVitesse(1);
                    enCoursSet(true);
                    menu_commencer_pause_variables.actif = 0;
                }
                else if(menu_commencer_pause_variables.selection==2) menu_commencer_pause_reglage(); //Réglage limité
                return 1;
            }
            else if(boutons.retour)
            {
                ajustementVitesse(1);
                enCoursSet(true);
                menuUpdate = true;
                boutons.retour = false;
                menu_commencer_pause_variables.actif = 0;
                menu_commencer_pause_variables.selection = 1;
            }
            if(menu_commencer_pause_variables.selection>menu_commencer_pause_variables.nbOption) menu_commencer_pause_variables.selection = 1;
            else if(menu_commencer_pause_variables.selection<1) menu_commencer_pause_variables.selection = menu_commencer_pause_variables.nbOption;

            if(menu_commencer_pause_variables.selection == 1)
            {
                affichageLCD(true, "Menu pause      ////>Continuer  Regl ");
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_commencer_pause_variables.selection == 2)
            {
                affichageLCD(true, "Menu pause      ////ntinuer >Reglage ");
                //                  1234567890123456/89/1234567890123456_
            }
        }
    }
    return 0;
}

bool menu_commencer_pause_reglage()
{
    nbBoutonsEnfonce = 0;
    menu_commencer_pause_reglage_variables.actif = 1;
    menu_commencer_pause_reglage_variables.nbOption = 3;

    if(menu_reglage_sensibilite_variables.actif) menu_reglage_sensibilite(); //réutilise le menu réglage de sensibilité
    else if(menu_reglage_mapReset_variables.actif) menu_reglage_mapReset(); //réutilise le menu réglage de resetMap
    else{
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_commencer_pause_reglage_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_commencer_pause_reglage_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_commencer_pause_reglage_variables.selection==1) // sensibilité
                {
                    sensibiliteModifie = detectionSensibiliteGet();
                    menu_reglage_sensibilite(); //réutilise le menu réglage de sensibilité
                }
                else if(menu_commencer_pause_reglage_variables.selection==2) // reset map
                {
                    menu_reglage_mapReset(); //réutilise le menu réglage de resetMap
                }
                else if(menu_commencer_pause_reglage_variables.selection==3) // retour
                {
                    menu_commencer_pause_reglage_variables.actif = 0;
                    menu_commencer_pause_reglage_variables.selection = 1;
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menuUpdate = true;
                boutons.retour = false;
                menu_commencer_pause_reglage_variables.actif = 0;
                menu_commencer_pause_reglage_variables.selection = 1;
            }
            if(menu_commencer_pause_reglage_variables.selection>menu_commencer_pause_reglage_variables.nbOption) menu_commencer_pause_reglage_variables.selection = 1;
            else if(menu_commencer_pause_reglage_variables.selection<1) menu_commencer_pause_reglage_variables.selection = menu_commencer_pause_reglage_variables.nbOption;

            if(menu_commencer_pause_reglage_variables.selection == 1)
            {
                affichageLCD(true, "Reglage reduit  ////>Sensibilite Res "); 
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_commencer_pause_reglage_variables.selection == 2)
            {
                affichageLCD(true, "Reglage reduit  ////e >ResetCarte Re ");
                //                  1234567890123456/89/1234567890123456_
            } 
            else if(menu_commencer_pause_reglage_variables.selection == 3)
            {
                affichageLCD(true, "Reglage reduit  ////setCarte >Retour ");
                //                  1234567890123456/89/1234567890123456_
            }         
        }
    }
    return 0;
}

bool menu_reglage()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_variables.actif = 1;
    menu_reglage_variables.nbOption = 4;
    if(menu_reglage_sensibilite_variables.actif) menu_reglage_sensibilite();
    else if(menu_reglage_dimension_variables.actif) menu_reglage_dimension();
    else if(menu_reglage_mapReset_variables.actif) menu_reglage_mapReset();
    else{
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_variables.selection==1) // sensibilité
                {
                    sensibiliteModifie = detectionSensibiliteGet();
                    menu_reglage_sensibilite();
                }
                else if(menu_reglage_variables.selection==2) // dimension
                {
                    dimensionXModifie = xGet();
                    dimensionYModifie = yGet();
                    menu_reglage_dimension();
                }
                else if(menu_reglage_variables.selection==3) // reset map
                {
                    menu_reglage_mapReset();
                }
                else if(menu_reglage_variables.selection==4) // retour
                {
                    menuUpdate = true;
                    menu_reglage_variables.actif = 0;
                    menu_reglage_variables.selection = 1;
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menuUpdate = true;
                boutons.retour = false;
                menu_reglage_variables.actif = 0;
                menu_reglage_variables.selection = 1;
            }
            if(menu_reglage_variables.selection>menu_reglage_variables.nbOption) menu_reglage_variables.selection = 1;
            else if(menu_reglage_variables.selection<1) menu_reglage_variables.selection = menu_reglage_variables.nbOption;

            if(menu_reglage_variables.selection == 1)
            {
                affichageLCD(true, "Menu reglage    ////>Sensibilite Dim "); 
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_variables.selection == 2)
            {
                affichageLCD(true, "Menu reglage    ////te >Dimension Re ");
                //                  1234567890123456/89/1234567890123456_
            }    
            else if(menu_reglage_variables.selection == 3)
            {
                affichageLCD(true, "Menu reglage    ////n >ResetCarte Re ");
                //                  1234567890123456/89/1234567890123456_
            } 
            else if(menu_reglage_variables.selection == 4)
            {
                affichageLCD(true, "Menu reglage    ////setCarte >Retour ");
                //                  1234567890123456/89/1234567890123456_
            }         
        }
    }
    return 0;
}

bool menu_reglage_sensibilite()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_sensibilite_variables.actif = 1;
    menu_reglage_sensibilite_variables.nbOption = 5;
    if(menu_reglage_sensibilite_confirmation_variables.actif) menu_reglage_sensibilite_confirmation();
    else
    {
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate)) //== 0?
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_sensibilite_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_sensibilite_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_sensibilite_variables.selection==1) // Annuler
                {
                    sensibiliteModifie = detectionSensibiliteGet();
                    menuUpdate = true;
                    menu_reglage_sensibilite_variables.actif = 0;
                    menu_reglage_sensibilite_variables.selection = 1;
                }
                else if(menu_reglage_sensibilite_variables.selection==2) // Confirmer
                {
                    menuUpdate = true;
                    menuTempsAccumule = 0;
                    menu_reglage_sensibilite_confirmation();
                }
                else if(menu_reglage_sensibilite_variables.selection==3) // +100
                {
                    sensibiliteModifie+=100;
                }
                else if(menu_reglage_sensibilite_variables.selection==4) // +10
                {
                    sensibiliteModifie+=10;
                }
                else if(menu_reglage_sensibilite_variables.selection==5) // +1
                {
                    sensibiliteModifie+=1;
                }

                if(menu_reglage_sensibilite_variables.selection > 2)
                {
                    if(sensibiliteModifie >= SENSIBILITE_MAX) sensibiliteModifie = sensibiliteModifie%100 + SENSIBILITE_MIN; 
                    else if(sensibiliteModifie < SENSIBILITE_MIN) sensibiliteModifie = SENSIBILITE_MIN; 
                }
                return 1;
            }
            else if(boutons.retour)
            {
            sensibiliteModifie = detectionSensibiliteGet();
                menuUpdate = true;
                boutons.retour = false;
                menu_reglage_sensibilite_variables.actif = 0;
                menu_reglage_sensibilite_variables.selection = 1;
            }
            if(menu_reglage_sensibilite_variables.selection>menu_reglage_sensibilite_variables.nbOption) menu_reglage_sensibilite_variables.selection = 1;
            else if(menu_reglage_sensibilite_variables.selection<1) menu_reglage_sensibilite_variables.selection = menu_reglage_sensibilite_variables.nbOption;

            if(menu_reglage_sensibilite_variables.selection == 1)
            {
                sprintf(str, "Sensibilite     ////>Annuler Confirm ");
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }
            else if(menu_reglage_sensibilite_variables.selection == 2)
            {
                sprintf(str, "Sensibilite     ////>Confirmer %icm ", sensibiliteModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_sensibilite_variables.selection == 3)
            {
                sprintf(str, "Sensibilitev    ////Confirmer >%icm ", sensibiliteModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_sensibilite_variables.selection == 4)
            {
                sprintf(str, "Sensibilite v   ////Confirmer >%icm ", sensibiliteModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_sensibilite_variables.selection == 5)
            {
                sprintf(str, "Sensibilite  v  ////Confirmer >%icm ", sensibiliteModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            } 
        }
    }
    return 0;
}

bool menu_reglage_dimension()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_dimension_variables.actif = 1;
    menu_reglage_dimension_variables.nbOption = 4;

    if(menu_reglage_dimension_x_variables.actif) menu_reglage_dimension_x();
    else if(menu_reglage_dimension_y_variables.actif) menu_reglage_dimension_y();
    else{
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_dimension_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_dimension_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_dimension_variables.selection==1) // X
                {
                    dimensionXModifie = xGet();
                    dimensionYModifie = yGet();
                    menu_reglage_dimension_x();
                }
                else if(menu_reglage_dimension_variables.selection==2) // Y
                {
                    dimensionXModifie = xGet();
                    dimensionYModifie = yGet();
                    menu_reglage_dimension_y();
                }
                else if(menu_reglage_dimension_variables.selection==3) // retour
                {
                    menuUpdate = true;
                    menu_reglage_dimension_variables.actif = 0;
                    menu_reglage_dimension_variables.selection = 1;
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menuUpdate = true;
                boutons.retour = false;
                menu_reglage_dimension_variables.actif = 0;
                menu_reglage_dimension_variables.selection = 1;
            }
            if(menu_reglage_dimension_variables.selection>menu_reglage_dimension_variables.nbOption) menu_reglage_dimension_variables.selection = 1;
            else if(menu_reglage_dimension_variables.selection<1) menu_reglage_dimension_variables.selection = menu_reglage_dimension_variables.nbOption;

            if(menu_reglage_dimension_variables.selection == 1)
            {
                affichageLCD(true, "Menu dimension  ////>X Y Retour      "); 
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_dimension_variables.selection == 2)
            {
                affichageLCD(true, "Menu dimension  ////X >Y Retour      "); 
                //                  1234567890123456/89/1234567890123456_
            }    
            else if(menu_reglage_dimension_variables.selection == 3)
            {
                affichageLCD(true, "Menu dimension  ////X Y >Retour      "); 
                //                  1234567890123456/89/1234567890123456_
            } 
        }
    }
    return 0;
}

bool menu_reglage_dimension_x()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_dimension_x_variables.actif = 1;
    menu_reglage_dimension_x_variables.nbOption = 5;
    
    if(menu_reglage_dimension_x_confirmation_variables.actif) menu_reglage_dimension_x_confirmation();
    else
    {

        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate)) //== 0?
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_dimension_x_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_dimension_x_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_dimension_x_variables.selection==1) // Annuler
                {
                    dimensionXModifie = xGet();
                    menuUpdate = true;
                    menu_reglage_dimension_x_variables.actif = 0;
                    menu_reglage_dimension_x_variables.selection = 1;
                }
                else if(menu_reglage_dimension_x_variables.selection==2) // Confirmer
                {
                    menuUpdate = true;
                    menuTempsAccumule = 0;
                    menu_reglage_dimension_x_confirmation();
                }
                else if(menu_reglage_dimension_x_variables.selection==3) // +100
                {
                    dimensionXModifie+=100;
                }
                else if(menu_reglage_dimension_x_variables.selection==4) // +10
                {
                    dimensionXModifie+=10;
                }
                else if(menu_reglage_dimension_x_variables.selection==5) // +2
                {
                    dimensionXModifie+=2;
                }

            if(menu_reglage_dimension_x_variables.selection > 2)
            {
                if(dimensionXModifie > DIMENSION_X_MAX) dimensionXModifie = dimensionXModifie%100 + DIMENSION_X_MIN; 
                else if(dimensionXModifie <= DIMENSION_X_MIN) dimensionXModifie = DIMENSION_X_MIN; 
            }
            return 1;
        }
        else if(boutons.retour)
        {
            dimensionXModifie = xGet();
            menuUpdate = true;
            boutons.retour = false;
            menu_reglage_dimension_x_variables.actif = 0;
            menu_reglage_dimension_x_variables.selection = 1;
        }
        if(menu_reglage_dimension_x_variables.selection>menu_reglage_dimension_x_variables.nbOption) menu_reglage_dimension_x_variables.selection = 1;
        else if(menu_reglage_dimension_x_variables.selection<1) menu_reglage_dimension_x_variables.selection = menu_reglage_dimension_x_variables.nbOption;

            if(menu_reglage_dimension_x_variables.selection == 1)
            {
                affichageLCD(true, "Dimension X     ////>Annuler Confirm "); 
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_dimension_x_variables.selection == 2)
            {
                sprintf(str, "Dimension X     ////>Confirmer %icm ", dimensionXModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_x_variables.selection == 3)
            {
                sprintf(str, "Dimension Xv    ////Confirmer >%icm ", dimensionXModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_x_variables.selection == 4)
            {
                sprintf(str, "Dimension X v   ////Confirmer >%icm ", dimensionXModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_x_variables.selection == 5)
            {
                sprintf(str, "Dimension X  v  ////Confirmer >%icm ", dimensionXModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            } 
        }
    }
    return 0;
}

bool menu_reglage_dimension_y()
{
    
    nbBoutonsEnfonce = 0;
    menu_reglage_dimension_y_variables.actif = 1;
    menu_reglage_dimension_y_variables.nbOption = 5;

    if(menu_reglage_dimension_y_confirmation_variables.actif) menu_reglage_dimension_y_confirmation();
    else
    {
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate)) //== 0?
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_dimension_y_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_dimension_y_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_dimension_y_variables.selection==1) // Annuler
                {
                    dimensionYModifie = yGet();
                    menuUpdate = true;
                    menu_reglage_dimension_y_variables.actif = 0;
                    menu_reglage_dimension_y_variables.selection = 1;
                }
                else if(menu_reglage_dimension_y_variables.selection==2) // Confirmer
                {
                        menuUpdate = true;
                        menuTempsAccumule = 0;
                        menu_reglage_dimension_y_confirmation();
                }
                else if(menu_reglage_dimension_y_variables.selection==3) // +100
                {
                    dimensionYModifie+=100;
                }
                else if(menu_reglage_dimension_y_variables.selection==4) // +10
                {
                    dimensionYModifie+=10;
                }
                else if(menu_reglage_dimension_y_variables.selection==5) // +2
                {
                    dimensionYModifie+=2;
                }

                if(menu_reglage_dimension_y_variables.selection > 2)
                {
                    if(dimensionYModifie > DIMENSION_Y_MAX) dimensionYModifie = dimensionYModifie%100 + DIMENSION_Y_MIN; 
                    else if(dimensionYModifie <= DIMENSION_Y_MIN) dimensionYModifie = DIMENSION_Y_MIN; 
                }
                return 1;
            }
            else if(boutons.retour)
            {
                dimensionYModifie = yGet();
                menuUpdate = true;
                boutons.retour = false;
                menu_reglage_dimension_y_variables.actif = 0;
                menu_reglage_dimension_y_variables.selection = 1;
            }
            if(menu_reglage_dimension_y_variables.selection>menu_reglage_dimension_y_variables.nbOption) menu_reglage_dimension_y_variables.selection = 1;
            else if(menu_reglage_dimension_y_variables.selection<1) menu_reglage_dimension_y_variables.selection = menu_reglage_dimension_y_variables.nbOption;

            if(menu_reglage_dimension_y_variables.selection == 1)
            {
                affichageLCD(true, "Dimension Y     ////>Annuler Confirm "); 
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_dimension_y_variables.selection == 2)
            {
                sprintf(str, "Dimension Y     ////>Confirmer %icm ", dimensionYModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_y_variables.selection == 3)
            {
                sprintf(str, "Dimension Yv    ////Confirmer >%icm ", dimensionYModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_y_variables.selection == 4)
            {
                sprintf(str, "Dimension Y v   ////Confirmer >%icm ", dimensionYModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            }    
            else if(menu_reglage_dimension_y_variables.selection == 5)
            {
                sprintf(str, "Dimension Y  v  ////Confirmer >%icm ", dimensionYModifie);
                //            1234567890123456/89/1234567890123456_
                affichageLCD(true, str); 
            } 
        }
    }
    return 0;
}

bool menu_reglage_mapReset()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_mapReset_variables.actif = 1;
    menu_reglage_mapReset_variables.nbOption = 2;
    if(menu_reglage_mapReset_confirmation_variables.actif) menu_reglage_mapReset_confirmation();
    else
    {

        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1 || (nbBoutonsEnfonce < 2 && menuUpdate))
        {
            menuUpdate = false;
            if(boutons.change_gauche)
            {
                menu_reglage_mapReset_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_reglage_mapReset_variables.selection++;
            }
            else if(boutons.select)
            {
                menuUpdate = true;
                boutons.select = false;
                if(menu_reglage_mapReset_variables.selection==1) // Annuler
                {
                    menu_reglage_mapReset_variables.actif = 0;
                }
                else if(menu_reglage_mapReset_variables.selection==2) // Confirmer
                {
                    menuUpdate = true;
                    menuTempsAccumule = 0;
                    menu_reglage_mapReset_confirmation();
                } 
                return 1;
            }
            else if(boutons.retour)
            {
                menuUpdate = true;
                boutons.retour = false;
                menu_reglage_mapReset_variables.actif = 0;
                menu_reglage_mapReset_variables.selection = 1;
            }
            if(menu_reglage_mapReset_variables.selection>menu_reglage_mapReset_variables.nbOption) menu_reglage_mapReset_variables.selection = 1;
            else if(menu_reglage_mapReset_variables.selection<1) menu_reglage_mapReset_variables.selection = menu_reglage_mapReset_variables.nbOption;
            
            if(menu_reglage_mapReset_variables.selection == 1)
            {
                affichageLCD(true, "Effacer la carte////>Annuler Confirm ");
                //                  1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_mapReset_variables.selection == 2)
            {
                affichageLCD(true, "Effacer la carte////nuler >Confirmer ");
                //                  1234567890123456/89/1234567890123456_
            }
        }
    }
    
    return 0;
}

bool menu_reglage_mapReset_confirmation() 
{
    menu_reglage_mapReset_confirmation_variables.actif = 1;

    if(menuUpdate){
        menuUpdate = false;
        dimensionXModifie = xGet();
        dimensionYModifie = yGet();
        resetCarte(dimensionXModifie, dimensionYModifie);
        affichageLCD(true, "CarteReset fait ////avec succes!     ");
        //                  1234567890123456/89/1234567890123456_
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_mapReset_confirmation_variables.actif = 0;
        menu_reglage_mapReset_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;

    return 0;
}

bool menu_reglage_sensibilite_confirmation()
{
    menu_reglage_sensibilite_confirmation_variables.actif = 1;
    detectionSensibiliteSet(sensibiliteModifie);

    if(menuUpdate){
        menuUpdate = false;
        sprintf(str, "Sensibilite     ////nouvelle = %icm  ", sensibiliteModifie);
        //            1234567890123456/89/1234567890123456_
        affichageLCD(true, str); 
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_sensibilite_confirmation_variables.actif = 0;
        menu_reglage_sensibilite_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;

    return 0;
}

bool menu_reglage_dimension_x_confirmation()
{
    menu_reglage_dimension_x_confirmation_variables.actif = 1;

    if(menuUpdate){
        menuUpdate = false;
        resetCarte(dimensionXModifie, dimensionYModifie);
        sprintf(str, "Dimension x     ////nouvelle = %icm  ", dimensionXModifie);
        //            1234567890123456/89/1234567890123456_
        affichageLCD(true, str); 
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_dimension_x_confirmation_variables.actif = 0;
        menu_reglage_dimension_x_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;

    return 0;
}

bool menu_reglage_dimension_y_confirmation()
{
    menu_reglage_dimension_y_confirmation_variables.actif = 1;
    
    if(menuUpdate){
        menuUpdate = false;
        resetCarte(dimensionXModifie, dimensionYModifie);
        sprintf(str, "Dimension y     ////nouvelle = %icm  ", dimensionYModifie);
        //            1234567890123456/89/1234567890123456_
        affichageLCD(true, str); 
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_dimension_y_confirmation_variables.actif = 0;
        menu_reglage_dimension_y_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;

    return 0;
}

bool menu_terminer()
{
    menu_variables.actif = 0;
    menu_commencer_variables.actif = 0;
    menu_commencer_pause_variables.actif = 0;
    menu_commencer_pause_reglage_variables.actif = 0;
    menu_reglage_variables.actif = 0;
    menu_reglage_sensibilite_variables.actif = 0;
    menu_reglage_dimension_variables.actif = 0;
    menu_reglage_mapReset_variables.actif = 0;

    nbBoutonsEnfonce = 0;
    menu_terminer_variables.actif = 1;
    menu_terminer_variables.nbOption = 1;

    if(menuTerminerTempsAccumule > menuTerminerTempsRequis) //== 0?
    {
        switch(menu_terminer_variables.selection)
        {
            case 1 :
                affichageLCD(true, "Fin de tache    ////Appuyer sur le b "); //Appuyer sur le bouton de reanitialisation "
                //                  1234567890123456/89/1234567890123456_      1234/6789/1234/6789/1234/6789012345678901
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= DELAIS/1.5;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 2 :
                affichageLCD(true, "Fin de tache    ////uyer sur le bout "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 3 :
                affichageLCD(true, "Fin de tache    ////r sur le bouton  "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 4 :
                affichageLCD(true, "Fin de tache    ////ur le bouton de  "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 5 :
                affichageLCD(true, "Fin de tache    ////le bouton de rea "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 6 :
                affichageLCD(true, "Fin de tache    ////bouton de reanit "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 7 :
                affichageLCD(true, "Fin de tache    ////ton de reanitial "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 8 :
                affichageLCD(true, "Fin de tache    //// de reanitialisa "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 9 :
                affichageLCD(true, "Fin de tache    //// reanitialisatio "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 10 :
                affichageLCD(true, "Fin de tache    ////reanitialisation "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= DELAIS/1.5;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 11 :
                affichageLCD(true, "Fin de tache    ////n de reanitialis "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 12 :
                affichageLCD(true, "Fin de tache    ////bouton de reanit "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 13 :
                affichageLCD(true, "Fin de tache    ////r le bouton de r "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 14 :
                affichageLCD(true, "Fin de tache    ////er sur le bouton "); 
                //                  1234567890123456/89/1234567890123456_
                menu_terminer_variables.selection++;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
            case 15 :
                affichageLCD(true, "Fin de tache    ////Appuyer sur le b "); //Appuyer sur le bouton de reanitialisation "
                //                  1234567890123456/89/1234567890123456_      1234/6789/1234/6789/1234/6789012345678901
                menu_terminer_variables.selection = 1;
                menuTerminerTempsAccumule -= menuTerminerTempsRequis;
                break;
        }
    }
    menuTerminerTempsAccumule += DELAIS;
    

    return 0;
}

/*
bool menu_terminer_confirmation()
{
    menu_terminer_variables.actif = 1;
    
    if(menuUpdate){
        menuUpdate = false;
        affichageLCD(true, "Reinitialisation////Reussi!          "); 
        //                  1234567890123456/89/1234567890123456_
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_terminer_confirmation_variables.actif = 0;
        menu_terminer_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;

    return 0;
}
*/