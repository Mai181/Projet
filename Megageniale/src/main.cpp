/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales
// bool verif;
int delais(500);

void setup(){

    void BoardInit();
    void DisplayInit();
    Serial.begin(9600);
    pinMode(50, OUTPUT);  // LED rouge
    pinMode(48, OUTPUT);  // LED verte
    pinMode(46, OUTPUT);  // LED orange
    INIT_servos();
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    delay(delais);
}

void loop(){

    // Convertion String en char
    /*String message = "Hello World!";
    char str_array[message.length()];
    message.toCharArray(str_array, message.length());
    char* token = strtok(str_array, " ");
    */

    allumerDEL(MARCHE, true);
    
    arbreDecision();
}
