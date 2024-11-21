/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales

int distributeurTempsAccumule=0;
int distributeurTempsRequis=3000;

void setup(){

    BoardInit();
    DisplayInit();
    Serial.begin(9600);
    Wire.begin();
    pinMode(PINROUGE, OUTPUT);  
    pinMode(PINVERT, OUTPUT);  
    pinMode(PINORANGE, OUTPUT);  
    pinMode(PIN_BOUTON_3_SELECT, INPUT);  
    pinMode(PIN_BOUTON_2_CHANGE, INPUT);  
    pinMode(PIN_SERVO_DISTRIBUTEUR, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_1, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_2, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_3, INPUT);  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    INIT_servos();
    allumerDEL(RIEN, true);
    affichageLCD("P-19 IronMinds");
    delay(DELAIS);
}

void loop(){

    // Test LCD
    // Convertion String en char
    /*
    affichageLCD("P-19 IronMinds");
    delay(DELAIS*100);
    */

    // Tests DEL couleurs
    /*
    int test = 3;
    allumerDEL(MARCHE, false);
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
    /*
    if(distributeurTempsAccumule>distributeurTempsRequis)
    {
        distributeur(true);
        distributeurTempsAccumule-=distributeurTempsRequis;
    }else
    {
        distributeur(false);
        distributeurTempsAccumule+=DELAIS;
    }
    delay(DELAIS);
    */

    //Test detecteur métaux
    /*
    Serial.print("detecteur metaux : ");
    Serial.println(detectionMetaux());
    delay(DELAIS);
    */

    //Test boutons
    /*
    */
    //Serial

    /*
    */
    boutonsUpdate();
    arbreDecision();
    delay(DELAIS);
}
