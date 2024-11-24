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
bool menuUpdate = true;
char str[37]; /** String temporaire*/

const int DIMENSION_MIN = 100;
const int DIMENSION_MAX = 200;
int dimensionXModifie = 100;
int dimensionYModifie = 100;

const int SENSIBILITE_MIN = 500;
const int SENSIBILITE_MAX = 1000;
int sensibiliteModifie = 600;

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
bool menu_reglage_dimension_x();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_x_variables;

/** Menu  */
bool menu_reglage_dimension_y();//menu_commencer_pause_reglage_dimension
/** Variables de navigation du menu principal */
MenuVariables menu_reglage_dimension_y_variables;

/** Menu  */
bool menu_reglage_mapReset();
/** Variables de navigation du menu_reglage_mapReset */
MenuVariables menu_reglage_mapReset_variables;

/** Menu  */
bool menu_reglage_mapReset_confirmation();
/** Variables de navigation du menu_reglage_mapReset */
MenuVariables menu_reglage_mapReset_confirmation_variables;

/** Menu */
bool menu_reglage_sensibilite_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_sensibilite_confirmation_variables;

/** Menu */
bool menu_reglage_dimension_x_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_dimension_x_confirmation_variables;

/** Menu */
bool menu_reglage_dimension_y_confirmation();
/** Variables de navigation du menu_reglage_mapReset_confirmation */
MenuVariables menu_reglage_dimension_y_confirmation_variables;

// Variables
int tempAccumuleAffichage = 0;
int tempRequisAffichage = 1000;
int menuTempsAccumule = 0;
int menuTempsRequis = 1000;
bool rougeAllume = false;
bool orangeAllume = false;
bool verteAllume = false;



/** 
* Fonction afficheur LCD
* @param clearBefore : (bool) Efface l'écritue préalablement si true
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
                    avancer();
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
    menu_commencer_variables.nbOption = 2;

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

/*à voir si à faire
bool menu_commencer_arreter()
{
    
}
*/

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
                    avancer();
                    enCoursSet(true);
                    menu_commencer_pause_variables.actif = 0;
                }
                else if(menu_commencer_pause_variables.selection==2) menu_commencer_pause_reglage(); //Réglage limité
                return 1;
            }
            else if(boutons.retour)
            {
                avancer();
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

/*à voir si à faire
bool menu_commencer_pause_arreter()
{
}
*/

/* Réutilisation des menu utilisé par le menu réglage
bool menu_commencer_pause_reglage_sensibilite()
{
    menu_reglage_sensibilite();
}
bool menu_commencer_pause_reglage_mapReset()
{
    menu_reglage_mapReset();
}
*/

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

bool menu_reglage_sensibilite()//menu_commencer_pause_reglage_sensibilite
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
                    if(sensibiliteModifie >= SENSIBILITE_MAX) sensibiliteModifie -= SENSIBILITE_MAX - SENSIBILITE_MIN; 
                    else if(sensibiliteModifie < SENSIBILITE_MIN) sensibiliteModifie = SENSIBILITE_MIN; 
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

bool menu_reglage_dimension()//menu_commencer_pause_reglage_dimension
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
                    menu_reglage_dimension_x();
                }
                else if(menu_reglage_dimension_variables.selection==2) // Y
                {
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

bool menu_reglage_dimension_x()//menu_commencer_pause_reglage_dimension
{
    nbBoutonsEnfonce = 0;
    menu_reglage_dimension_x_variables.actif = 1;
    menu_reglage_dimension_x_variables.nbOption = 5;

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
                resetCarte(dimensionXModifie/2, dimensionYModifie/2);
                menuUpdate = true;
                menu_reglage_dimension_x_variables.actif = 0;
                menu_reglage_dimension_x_variables.selection = 1;
            }
            else if(menu_reglage_dimension_x_variables.selection==3) // +100
            {
                dimensionXModifie+=100;
            }
            else if(menu_reglage_dimension_x_variables.selection==4) // +10
            {
                dimensionXModifie+=10;
            }
            else if(menu_reglage_dimension_x_variables.selection==5) // +1
            {
                dimensionXModifie+=1;
            }

            if(menu_reglage_dimension_x_variables.selection > 2)
            {
                if(dimensionXModifie > DIMENSION_MAX) dimensionXModifie -= DIMENSION_MAX - DIMENSION_MIN; 
                else if(dimensionXModifie < DIMENSION_MIN) dimensionXModifie = DIMENSION_MIN; 
            }
            return 1;
        }
        else if(boutons.retour)
        {
            menuUpdate = true;
            boutons.retour = false;
            menu_reglage_dimension_x_variables.actif = 0;
            menu_reglage_dimension_x_variables.selection = 1;
        }
        if(menu_reglage_dimension_x_variables.selection>menu_reglage_dimension_x_variables.nbOption) menu_reglage_dimension_x_variables.selection = 1;
        else if(menu_reglage_dimension_x_variables.selection<1) menu_reglage_dimension_x_variables.selection = menu_reglage_dimension_x_variables.nbOption;

        if(menu_reglage_dimension_x_variables.selection == 1)
        {
            affichageLCD(true, "Dimension x     ////>Annuler Confirm "); 
            //                  1234567890123456/89/1234567890123456_
        }
        else if(menu_reglage_dimension_x_variables.selection == 2)
        {
            sprintf(str, "Dimension x     ////>Confirmer %icm ", dimensionXModifie);
            //            1234567890123456/89/1234567890123456_
            affichageLCD(true, str); 
        }    
        else if(menu_reglage_dimension_x_variables.selection == 3)
        {
            sprintf(str, "Dimension xv    ////Confirmer >%icm ", dimensionXModifie);
            //            1234567890123456/89/1234567890123456_
            affichageLCD(true, str); 
        }    
        else if(menu_reglage_dimension_x_variables.selection == 4)
        {
            sprintf(str, "Dimension x v   ////Confirmer >%icm ", dimensionXModifie);
            //            1234567890123456/89/1234567890123456_
            affichageLCD(true, str); 
        }    
        else if(menu_reglage_dimension_x_variables.selection == 5)
        {
            sprintf(str, "Dimension x  v  ////Confirmer >%icm ", dimensionXModifie);
            //            1234567890123456/89/1234567890123456_
            affichageLCD(true, str); 
        } 
    }
    return 0;
}

bool menu_reglage_dimension_y()//menu_commencer_pause_reglage_dimension
{
    
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
    resetCarte(carteNbColonneModifie, carteNbLigneModifie);

    if(menuUpdate){
        menuUpdate = false;
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
}

bool menu_reglage_sensibilite_confirmation()
{
    menu_reglage_sensibilite_confirmation_variables.actif = 1;
    detectionSensibiliteSet(sensibiliteModifie);

    if(menuUpdate){
        menuUpdate = false;
        sprintf(str, "Sensibilite     ////nouvelle = %icm", sensibiliteModifie);
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
}

bool menu_reglage_dimension_x_confirmation()
{
    menu_reglage_dimension_x_confirmation_variables.actif = 1;

    if(menuUpdate){
        menuUpdate = false;
        affichageLCD(true, "Dimension x     ////nouvelle = 222cm ");
        //                  1234567890123456/89/1234567890123456_
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_dimension_x_confirmation_variables.actif = 0;
        menu_reglage_dimension_x_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;
}

bool menu_reglage_dimension_y_confirmation()
{
    menu_reglage_dimension_y_confirmation_variables.actif = 1;

    if(menuUpdate){
        menuUpdate = false;
        affichageLCD(true, "Dimension y     ////nouvelle = 222cm ");
        //                  1234567890123456/89/1234567890123456_
    }
    if(menuTempsAccumule > menuTempsRequis){
        menuUpdate = true;
        tempAccumuleAffichage -= tempRequisAffichage;
        menu_reglage_dimension_y_confirmation_variables.actif = 0;
        menu_reglage_dimension_y_variables.actif = 0;
    }
    else menuTempsAccumule += DELAIS;
}
// else if(menu_reglage_dimension_x_confirmation_variables.actif) menu_reglage_dimension_x_confirmation();
// else if(menu_reglage_dimension_y_confirmation_variables.actif) menu_reglage_dimension_y_confirmation();