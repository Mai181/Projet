/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales
const int delais = 20;

void setup(){

    void BoardInit();
    void DisplayInit();
}

void loop(){

    String message = "Hello World!";
    char str_array[message.length()];
    message.toCharArray(str_array, message.length());
    char* token = strtok(str_array, " ");
    bool verif;
    verif = affichageLCD(token);
    Serial.println(verif);
    delay(delais);
}
