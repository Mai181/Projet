/*
    Fichier : main.cpp
    Auteurs : P-19
    
    Date: 17 octobre 2024
    Description: Trouve les objets dans une arène 
                et va les porter dans les zones de 
                couleur
 */

#include <Arduino.h>
#include <librobus.h>
#include <GroveColorSensor.h>
#include <stdint.h>



/********** 
 * Début de la zone des variables et constantes */
//
GroveColorSensor colorSensor;
int numTest = 5;  // nb of tests


/**Délai en ms entre chaque itération du loop */

const int DT=50;
/** Boucle de débug */
const bool DEBUGAGE=true;


/********** FIN de la zone des variables et constantes
 * Début de la zone des fonctions */
//
/**Fonction détecte couleur et retourne valeur entre -1 et 3 selon s'il y a un erreur ou selon la couleur détectée
 * @return -1=inconnu, 0=red, 1=green, 2=blue, 3=yellow
*/
int detectColor() {
    int redSum = 0, greenSum = 0, blueSum = 0;
    int color= -1;

    // Multiple tests
    for (int i = 0; i < numTest; i++) {
        int red, green, blue;
        colorSensor.readRGB(&red, &green, &blue);  // Read RGB values

        redSum += red;  //sum tests
        greenSum += green;
        blueSum += blue;

        delay(50);
    }
    
    // Calculate avg
    int redAvg = redSum / numTest;
    int greenAvg = greenSum / numTest;
    int blueAvg = blueSum / numTest;

    // Determine color based on avg
    if (redAvg > greenAvg && redAvg > blueAvg) {
        Serial.print("Detected Color: Red");
        color= 0;
    } else if (greenAvg > redAvg && greenAvg > blueAvg) {
        Serial.print("Detected Color: Green");
        color= 1;
    } else if (blueAvg > redAvg && blueAvg > greenAvg) {
        Serial.print("Detected Color: Blue");
        color= 2;
    } else if (redAvg > greenAvg && greenAvg > blueAvg) {
        Serial.print("Detected Color: Yellow");
        color= 3; // Yellow detected as red + green
    } else {
        Serial.print("Detected Color: Unknown");
    }
    colorSensor.clearInterrupt();
    Serial.println("detectColor finished");
    return color;
}
    /**Lorsque la fonction est appelée, soit que les servomoteurs sont fermés (45) ou ouverts (135)
     */
void SERVO_ouvert(bool ouvert) {
    
    uint8_t servoAngle = 45;	// angle servomoteur lorsque fermé
        if (ouvert){
            servoAngle = 135;   // angle servomoteur lorsque ouvert
        }

    SERVO_SetAngle(0, servoAngle);  // Servomoteur gauche
    SERVO_SetAngle(1, 180 - servoAngle); // Servomoteur droit
}
void INIT_servos(){
    SERVO_Enable(0);
    SERVO_Enable(1);
    SERVO_ouvert(true);
}




/********** FIN de la zone des fonctions
 * Début du main */
//

/** Fonction de départ, se fait appeler une seule fois au début du programme*/

void setup(){
	BoardInit();
    Serial.begin(9600); //Communication à 9600 bits/sec
    Serial.println("Setup started");
    delay(10);
    MOTOR_SetSpeed(0,-0.25);
    delay(100);
    //Wire.begin();
    //colorSensor.ledStatus = 1;
    //INIT_servos();
    Serial.println("Setup finished");
}

/** Fonction de départ, se fait appeler à chaque fois qu'elle est terminée */

void loop(){
    //boucle de test : code temporaire qui peut être remplacé et effacé
    Serial.println("loop started");
    while(DEBUGAGE){
        //code temporaire qui peut être remplacé et effacé
        Serial.println("loop test started");
        MOTOR_SetSpeed(1,0.5);
        Serial.print("Couleur détectée : ");
        Serial.println(detectColor());
        /*
        delay(500);
        Serial.print("mouvement servo ouvert ");
        SERVO_ouvert(true);
        delay(1000);
        Serial.print("mouvement servo fermé ");
        SERVO_ouvert(false);
        */
        Serial.println("loop test finished");
        delay(500);
    }
    //fin boucle de test



    Serial.println("loop finished");
    delay(DT);
}