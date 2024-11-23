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
int carteNbLigneModifie = 100;
int carteNbColonneModifie = 100;

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
                menu_variables.selection--;
            }
            else if(boutons.change_droite)
            {
                menu_variables.selection++;
            }
            else if(boutons.select)
            {
                if(menu_variables.selection==1) //Commencer
                {
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
                affichageLCD("Menu principal  ////>Commencer Regla ");
                //            1234567890123456/89/1234567890123456_
            }
            else if(menu_variables.selection == 2)
            {
                affichageLCD("Menu principal  ////mmencer >Reglage ");
                //            1234567890123456/89/1234567890123456_
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

        if(nbBoutonsEnfonce == 1)
        {
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
                if(menu_commencer_variables.selection==1) //Pause
                {
                    arreter();
                    menu_commencer_pause();
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menu_commencer_variables.actif = 0;
            }
            if(menu_commencer_variables.selection>menu_commencer_variables.nbOption) menu_commencer_variables.selection = 1;
            else if(menu_commencer_variables.selection<1) menu_commencer_variables.selection = menu_commencer_variables.nbOption;

            if(menu_commencer_variables.selection == 1)
            {
                affichageLCD("Menu            ////En cours: >Pause ");
                //            1234567890123456/89/1234567890123456_
            }
        }
    }
    return 0;
}

//à voir si à faire
bool menu_commencer_arreter()
{
    
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

        if(nbBoutonsEnfonce == 1)
        {
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
                if(menu_commencer_pause_variables.selection==1) //Continuer
                {
                    avancer();
                    menu_commencer_pause_variables.actif = 0;
                }
                else if(menu_commencer_pause_variables.selection==2) menu_commencer_pause_reglage(); //Réglage limité
                return 1;
            }
            else if(boutons.retour)
            {
                avancer();
                menu_commencer_pause_variables.actif = 0;
                menu_commencer_pause_variables.selection = 1;
            }
            if(menu_commencer_pause_variables.selection>menu_commencer_pause_variables.nbOption) menu_commencer_pause_variables.selection = 1;
            else if(menu_commencer_pause_variables.selection<1) menu_commencer_pause_variables.selection = menu_commencer_pause_variables.nbOption;

            if(menu_commencer_pause_variables.selection == 1)
            {
                affichageLCD("Menu pause      ////>Continuer  Regl ");
                //            1234567890123456/89/1234567890123456_
            }
            else if(menu_commencer_pause_variables.selection == 2)
            {
                affichageLCD("Menu pause      ////ntinuer >Reglage ");
                //            1234567890123456/89/1234567890123456_
            }
        }
    }
    return 0;
}

bool menu_commencer_pause_reglage()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_variables.actif = 1;
    menu_reglage_variables.nbOption = 4;

    if(menu_reglage_sensibilite_variables.actif) menu_reglage_sensibilite(); //réutilise le menu réglage de sensibilité
    else if(menu_reglage_mapReset_variables.actif) menu_reglage_mapReset(); //réutilise le menu réglage de resetMap
    else{
        if(boutons.change_gauche) nbBoutonsEnfonce++;
        if(boutons.change_droite) nbBoutonsEnfonce++;
        if(boutons.select) nbBoutonsEnfonce++;
        if(boutons.retour) nbBoutonsEnfonce++;

        if(nbBoutonsEnfonce == 1)
        {
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
                if(menu_commencer_pause_reglage_variables.selection==1) // sensibilité
                {
                    menu_reglage_sensibilite(); //réutilise le menu réglage de sensibilité
                }
                else if(menu_commencer_pause_reglage_variables.selection==2) // reset map
                {
                    menu_reglage_mapReset(); //réutilise le menu réglage de resetMap
                }
                else if(menu_commencer_pause_reglage_variables.selection==3) // retour
                {
                    menu_commencer_pause_reglage_variables.selection = 1;
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menu_commencer_pause_reglage_variables.selection = 1;
            }
            if(menu_commencer_pause_reglage_variables.selection>menu_commencer_pause_reglage_variables.nbOption) menu_commencer_pause_reglage_variables.selection = 1;
            else if(menu_commencer_pause_reglage_variables.selection<1) menu_commencer_pause_reglage_variables.selection = menu_commencer_pause_reglage_variables.nbOption;

            if(menu_commencer_pause_reglage_variables.selection == 1)
            {
                affichageLCD("Menu reglage    ////>Sensibilite Res "); 
                //            1234567890123456/89/1234567890123456_
            }
            else if(menu_commencer_pause_reglage_variables.selection == 2)
            {
                affichageLCD("Menu reglage    ////>ResetCarte Reto ");
                //            1234567890123456/89/1234567890123456_
            } 
            else if(menu_commencer_pause_reglage_variables.selection == 3)
            {
                affichageLCD("Menu reglage    ////>Retour          ");
                //            1234567890123456/89/1234567890123456_
            }         
        }
    }
    return 0;
}

//À voir si à faire
bool menu_commencer_pause_arreter()
{
}

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

        if(nbBoutonsEnfonce == 1)
        {
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
                if(menu_reglage_variables.selection==1) // sensibilité
                {
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
                    menu_reglage_variables.selection = 1;
                }
                return 1;
            }
            else if(boutons.retour)
            {
                menu_reglage_variables.selection = 1;
            }
            if(menu_reglage_variables.selection>menu_reglage_variables.nbOption) menu_reglage_variables.selection = 1;
            else if(menu_reglage_variables.selection<1) menu_reglage_variables.selection = menu_reglage_variables.nbOption;

            if(menu_reglage_variables.selection == 1)
            {
                affichageLCD("Menu reglage    ////>Sensibilite Dim "); 
                //            1234567890123456/89/1234567890123456_
            }
            else if(menu_reglage_variables.selection == 2)
            {
                affichageLCD("Menu reglage    ////>Dimension Reset ");
                //            1234567890123456/89/1234567890123456_
            }    
            else if(menu_reglage_variables.selection == 3)
            {
                affichageLCD("Menu reglage    ////>ResetCarte Reto ");
                //            1234567890123456/89/1234567890123456_
            } 
            else if(menu_reglage_variables.selection == 4)
            {
                affichageLCD("Menu reglage    ////>Retour          ");
                //            1234567890123456/89/1234567890123456_
            }         
        }
    }
    return 0;
}

bool menu_reglage_sensibilite()//menu_commencer_pause_reglage_sensibilite
{

}

bool menu_reglage_dimension()//menu_commencer_pause_reglage_dimension
{

}

bool menu_reglage_mapReset()
{
    nbBoutonsEnfonce = 0;
    menu_reglage_mapReset_variables.actif = 1;
    menu_reglage_mapReset_variables.nbOption = 2;
    
    if(boutons.change_gauche) nbBoutonsEnfonce++;
    if(boutons.change_droite) nbBoutonsEnfonce++;
    if(boutons.select) nbBoutonsEnfonce++;
    if(boutons.retour) nbBoutonsEnfonce++;

    if(nbBoutonsEnfonce == 1)
    {
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
            if(menu_reglage_mapReset_variables.selection==1) // Annuler
            {
                menu_reglage_mapReset_variables.actif = 0;
            }
            else if(menu_reglage_mapReset_variables.selection==2) // Confirmer
            {
                resetCarte(carteNbColonneModifie, carteNbLigneModifie);
                menu_reglage_mapReset_variables.actif = 0;
                menu_reglage_mapReset_variables.selection=1;
            } 
            return 1;
        }
        else if(boutons.retour)
        {
            menu_reglage_mapReset_variables.actif = 0;
            menu_reglage_mapReset_variables.selection=1;
        }
        if(menu_reglage_mapReset_variables.selection>menu_reglage_mapReset_variables.nbOption) menu_reglage_mapReset_variables.selection = 1;
        else if(menu_reglage_mapReset_variables.selection<1) menu_reglage_mapReset_variables.selection = menu_reglage_mapReset_variables.nbOption;
        
        if(menu_reglage_mapReset_variables.selection == 1)
        {
            affichageLCD("Effacer la carte////>Annuler Confirm ");
            //            1234567890123456/89/1234567890123456_
        }
        else if(menu_reglage_mapReset_variables.selection == 2)
        {
            affichageLCD("Effacer la carte////nuler >Confirmer ");
            //            1234567890123456/89/1234567890123456_
        }
    }
    return 0;
}