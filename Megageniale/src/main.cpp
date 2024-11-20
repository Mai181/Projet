/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales

void setup(){

    BoardInit();
    DisplayInit();
    Serial.begin(9600);
    pinMode(PINROUGE, OUTPUT);  
    pinMode(PINVERT, OUTPUT);  
    pinMode(PINORANGE, OUTPUT);  
    pinMode(PIN_BOUTON_SELECT, INPUT);  
    pinMode(PIN_BOUTON_CHANGE, INPUT);  
    pinMode(PIN_SERVO_DISTRIBUTEUR, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX, INPUT);  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    allumerDEL(RIEN, true);
    // affichageLCD(MENU_INI_Y);
    delay(DELAIS);
}

void loop(){

    // Test LCD
    // Convertion String en char
    /*
    affichageLCD("P-19 20/11/2024");
    delay(DELAIS*100);
    */

    // Tests DEL couleurs
    /*int test = 1;
    allumerDEL(MARCHE, true);
    if (test == 1) allumerDEL(METAL, true);
    else if (test == 2) allumerDEL(OBSTACLE, true);
    else if (test == 3) allumerDEL(TERMINER, true);
    delay(DELAIS);
    */

    // Tests détecteurs métaux
    /*bool verif;
    verif = detectionMetaux();
    if (verif) Serial.println("Metal detecte!");
    else Serial.println("Aucun metal detecte!");
    */

    // Tests distributeur
    /*distributeur(true);
    delay(DELAIS*100);
    distributeur(false);
    */

    /*
    boutonsUpdate();
    arbreDecision();
    delay(DELAIS);
    */
}
