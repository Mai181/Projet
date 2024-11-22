/*    
*   Fichier: deplacement.cpp
*   Auteurs: P-19
*   Date: 14 novembre 2024
*   Description: implémentation des fonctioins de déplacament
*/

#include "header.h"

// Variables 
float erreurAccumuleeDroite = 0.0;
const float ki = 0.000525; // 0.0005
const float kp = 0.001525; // 0.0012

/**
* Fonction avancement
*/
void avancer(){
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    MOTOR_SetSpeed(LEFT, vitesseGauche);
    MOTOR_SetSpeed(RIGHT, vitesseDroite);
}

/**
* Fonction arret
*/
void arreter(){
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

/** Fonction pour limiter une valeur dans une plage donnée
 *  
 *  @param valeur: valeur (float)
 *  @param minVal: son minimum (float)
 *  @param maxVal: son maximum (float)
 *  
 *  @return minVal si < valeur, maxVal si > valeur ou valeur 
*/
float limiter(float valeur, float minVal, float maxVal) {
    if (valeur > maxVal) return maxVal;
    if (valeur < minVal) return minVal;
    return valeur;
}

/**
* Fonction ajustement vitesse
*/
void ajustementVitesse(){
    float totalpulseDroit = ENCODER_Read(RIGHT);
    float totalpulseGauche = ENCODER_Read(LEFT);
    float ecartDroit;
    float correctionDroit;

    if(vitesseDroite > 0){
        // Calcul des écarts entre la consigne et la mesure
        ecartDroit = totalpulseGauche - totalpulseDroit;
    }
    else{
        ecartDroit = totalpulseDroit - totalpulseGauche;
    }

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * kp;

    // Mise à jour des erreurs accumulées pour l'intégrale
    erreurAccumuleeDroite += ecartDroit * 0.05;

    // Calcul des termes intégrals
    float termeIntDroit = erreurAccumuleeDroite * ki;
    if(vitesseDroite > 0){
        // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
        correctionDroit = limiter(vitesseDroite + termePropDroit + termeIntDroit, -1.0, 1.0);
    }
    else{
        // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
        correctionDroit = limiter(vitesseDroite - termePropDroit - termeIntDroit, -1.0, 1.0);
    }

    // Application des corrections aux moteurs
    MOTOR_SetSpeed(RIGHT, correctionDroit);
    MOTOR_SetSpeed(LEFT, vitesseGauche);
}

/**
* Fonction rotation
* @param direction: (int) tourner à gauche ou à droite
*/
void rotation(int direction){
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    if(direction){
        MOTOR_SetSpeed(RIGHT, 0);
        MOTOR_SetSpeed(LEFT, vitesseGauche);
        while (ENCODER_Read(LEFT) <= fuckyou*29.202*0.66){}
        arreter();
    }
    else{
        MOTOR_SetSpeed(LEFT, 0);
        MOTOR_SetSpeed(RIGHT, vitesseDroite);
        while (ENCODER_Read(RIGHT) <= fuckyou*29.202*0.66){}
        arreter();
    }
}

