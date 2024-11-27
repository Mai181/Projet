/*    
*   Fichier: detection.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctions de déplacement
*/

#include "header.h"

int detectionSensibilite=500;
struct Boutons boutonsDetection;
struct Boutons boutonsBuffer;
struct Boutons boutonsRelease;
int distances[5] = {0};
int compteur = 0;
float moyenne = 0.0; 

/**
* Fonction détection métaux
* @return true si un métal est détecté
*/
bool detectionMetaux(){
    /*
    Serial.print("metaux 1 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_1));
    Serial.print("metaux 2 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_2));
    Serial.print("metaux 3 : ");
    Serial.println(analogRead(PIN_ANALOG_DETECTEUR_METAUX_3));
    */
    if(analogRead(PIN_ANALOG_DETECTEUR_METAUX_1)>detectionSensibilite || analogRead(PIN_ANALOG_DETECTEUR_METAUX_2)>detectionSensibilite || analogRead(PIN_ANALOG_DETECTEUR_METAUX_3)>detectionSensibilite)//à redéterminer la pin et tester la sensibilité (si 300 est trop élevé ou pas assez)
        return true;
    else
        return false;
}


/**
* Fonction mise à jour des boutons sélectionnés 
*/
void boutonsUpdate(){
    boutonsRelease.change_gauche = false;
    boutonsDetection.change_gauche=digitalRead(PIN_BOUTON_1_CHANGE_GAUCHE);
    if(boutonsBuffer.change_gauche && !boutonsDetection.change_gauche) boutonsRelease.change_gauche = true;
    boutonsBuffer.change_gauche = boutonsDetection.change_gauche;

    boutonsRelease.change_droite = false;
    boutonsDetection.change_droite=digitalRead(PIN_BOUTON_2_CHANGE_DROITE);
    if(boutonsBuffer.change_droite && !boutonsDetection.change_droite) boutonsRelease.change_droite = true;
    boutonsBuffer.change_droite = boutonsDetection.change_droite;

    boutonsRelease.select = false;
    boutonsDetection.select=digitalRead(PIN_BOUTON_3_SELECT);
    if(boutonsBuffer.select && !boutonsDetection.select) boutonsRelease.select = true;
    boutonsBuffer.select = boutonsDetection.select;

    boutonsRelease.retour = false;
    boutonsDetection.retour=digitalRead(PIN_BOUTON_4_RETOUR);
    if(boutonsBuffer.retour && !boutonsDetection.retour) boutonsRelease.retour = true;
    boutonsBuffer.retour = boutonsDetection.retour;
}

/**
* Fonction retourne l'état des boutons
*
* @return une structure avec l'état de tous les boutons mis à jour
*/
struct Boutons boutonsGet(){
    return boutonsRelease;
}

/** Donne la distance avec l'obstacle/objet devant, plus grande 
 * précision à 10cm suivi d'une imprécision grandissante en augmentant 
 * la distance et énormément grandissante en réduisant la distance
 * 
 * @return Une valeur réelle correspondant à la distance entre le 
 * capteur et l'objet
*/
float distanceObjet(){ 
    distances[compteur] = analogRead(A6);                      // CHANGER LA PIN !!!!!!!!!!!!!!!!!!!!!!!!!
    compteur++;
    if (compteur > 4){
        compteur = 0;
    }
    for (int i = 0; i < 5; i++){
        moyenne+=distances[i]; 
    }
    moyenne/=5.0;
    return ((((1/(moyenne))-0.0008)/0.0002)*(4.0/3.0));
    //Calcul traduisant la valeur analog en cm, document avec les calculs disponible sur Teams
    // return 10*(25-(sqrtf(10)*sqrtf(63*(res/((float)test))-2500)/sqrtf(res/((float)test))))-1.34445983;
}

int detectionSensibiliteGet()
{
    return detectionSensibilite;
}

void detectionSensibiliteSet(int sensibiliteTemp)
{
    detectionSensibilite = sensibiliteTemp;
}
