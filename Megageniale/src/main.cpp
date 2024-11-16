/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

// Variables globales
const int delais = 20;
float tempAccumule = 0;
float tempRequis = 1000;
bool verif;

void setup(){

    void BoardInit();
    void DisplayInit();

    pinMode(50, OUTPUT);  // LED rouge
    pinMode(48, OUTPUT);  // LED verte
    pinMode(46, OUTPUT);  // LED orange
}

void loop(){

    // Convertion String en char
    String message = "Hello World!";
    char str_array[message.length()];
    message.toCharArray(str_array, message.length());
    char* token = strtok(str_array, " ");

    if(tempAccumule > tempRequis){
        tempAccumule -= tempRequis;
        allumerDEL(1, true);
    }
    else {
        tempAccumule += delais;
    }
}
