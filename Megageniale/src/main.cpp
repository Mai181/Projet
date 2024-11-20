/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales
// bool verif;

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
    affichageLCD(MENU_INI_Y);
    delay(DELAIS);
}

void loop(){

    // Test LCD
    // Convertion String en char
    /*String message = "Hello World!";
    char str_array[message.length()];
    message.toCharArray(str_array, message.length());
    char* token = strtok(str_array, " ");
    delay(DELAIS * 250);
    affichageLCD(token);
    Serial.println(token);
    */

    // Tests DEL couleurs
    /*int test = 1;
    allumerDEL(MARCHE, true);
    if (test == 1) allumerDEL(METAL, true);
    else if (test == 2) allumerDEL(OBSTACLE, true);
    else if (test == 3) allumerDEL(TERMINER, true);
    delay(DELAIS);
    */


    boutonsUpdate();
    arbreDecision();
    delay(DELAIS);
}
