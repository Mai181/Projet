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
#include <Adafruit_TCS34725.h>
#include <stdint.h>

float cirCerRot = 58.0;  // circonferenceCercleRotation
float cirRoue = 23.94;  // circonferenceRoue
float tour = 360.0;
float pulseTourRoue = 3200.0; 
float vitesseRotationNeg = -0.17;  // vitesse négative du moteur 
float vitesseRotationPos = 0.17;  // vitesse positive du moteur
/********** 
 * Début de la zone des variables et constantes */
//
Adafruit_TCS34725 colorSensor;
int numTest = 5;  // nb of tests

int dataSuiveurLigne[3];
/** Délai en ms entre chaque itération du loop */
const int DT=50;
/** Boucle de débug */
const bool DEBUGAGE=true;

/********** FIN de la zone des variables et constantes
 * Début de la zone des fonctions */
//
/**Fonction détecte couleur et retourne couleur détectée ou erreur selon s'il y a un erreur ou selon la couleur détectée
 * @return "E"=inconnu, "R"=red, "V"=green, "B"=blue, "J"=yellow
*/
char detectColor() {
    int redSum = 0, greenSum = 0, blueSum = 0;
    char color = 'E';
    int clear;

    // Multiple tests
    for (int i = 0; i < numTest; i++) {
        int red, green, blue;
        colorSensor.getRawData(&red, &green, &blue, &clear);  // Read RGB values

        redSum += red;  //sum tests
        greenSum += green;
        blueSum += blue;

        delay(50);
    }
    
    // Calculate avg
    float redAvg = redSum / (float)numTest;
    float greenAvg = greenSum / (float)numTest;
    float blueAvg = blueSum / (float)numTest;
    Serial.println("Red");
    Serial.println(redAvg);
    Serial.println("green");
    Serial.println(greenAvg);
    Serial.println("blue");
    Serial.println(blueAvg);
    Serial.println("clear");
    Serial.println(clear);

    // Determine color based on avg
    if (redSum>10 && greenSum>10 &&blueSum>10){
        Serial.print("Detected Color: Unknown");
    } else if (redAvg >= greenAvg && greenAvg > blueAvg) {
        Serial.print("Detected Color: Yellow");
        color = 'J'; // Yellow detected as red + green
    } else if (greenAvg > redAvg && greenAvg > blueAvg) {
        Serial.print("Detected Color: Green");
        color = 'V';
    } else if (blueAvg > redAvg && blueAvg > greenAvg) {
        Serial.print("Detected Color: Blue");
        color = 'B';
    } else if (redAvg > greenAvg && redAvg > blueAvg) {
        Serial.print("Detected Color: Red");
        color = 'R';
    } else {
        Serial.print("Detected Color: Unknown");
    }
    colorSensor.clearInterrupt();
    Serial.println("detectColor finished");
    return color;
}

/*Lorsque la fonction est appelée, soit que les servomoteurs sont fermés (45) ou ouverts (135)*/
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

/**Écrit un tableau des retour des capteurs
 * 0=noir
 * 1=blanc
 */
void suiveurLigne(){
    dataSuiveurLigne[0]=digitalRead(52);
    dataSuiveurLigne[1]=digitalRead(50);
    dataSuiveurLigne[2]=digitalRead(48);
}

int detectionObjet(){
    /*
    int capteurHaut=analogRead(A7);
    int capteurBas=analogRead(A6);

    Serial.print("captDisHaut : ");
    Serial.println(capteurHaut);
    Serial.print("captDisBas : ");
    Serial.println(capteurBas);
    if (capteurHaut+20>capteurBas&&capteurBas+20>capteurHaut){
        return -1;
    }

    return capteurHaut-capteurBas;
    */
    float res=0;
    int current=0;
    int test=25;
    int min=analogRead(A6);
    int max=analogRead(A6);
    for(int i=test;i>0;i--){
        current=analogRead(A6);
        if(min>current){
            min=current;
        }else if (max<current){
            max=current;
        }
        res+=current;
        delay(5000/test);
    }
    Serial.print("min : ");
    Serial.println(min);
    Serial.print("max : ");
    Serial.println(max);
    return res/(float)test;
}

// Fonction direction en fonction de la couleur (en degré)
// Retourne int (0, 90, 180, 270)
int directionCouleur(){
    int direction;
    char current = detectColor();
    while (current == 'E'){
        current = detectColor();
    }
    if (current == 'R') {
        direction = 180;
    }
    else if (current == 'J'){
        direction = 90;
    }
    else if (current == 'B'){
        direction = 0;
    }
    else {
        direction = 270;
    }

    return direction;
}
// Fonction pour limiter une valeur dans une plage donnée
float limiterRot(float valeur, float minVal, float maxVal) {
    if (valeur > maxVal) return maxVal;
    if (valeur < minVal) return minVal;
    return valeur;
}

// Lecture de la vitesse de la roue droite
float lireVitesseDroitRot() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(10);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 10.0;
    return vitesse;
}

// Lecture de la vitesse de la roue gauche
float lireVitesseGaucheRot() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(10);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 10.0;
    return vitesse;
}

// Fonction de régulation PI pour ajuster les vitesses des moteurs
void CorrigerVitesseRot(float vd, float vg, int tourneDroit, int tourneGauche, float RerreurAccumuleeDroite) {
    float vitesseDroit = lireVitesseDroitRot();
    float vitesseGauche = lireVitesseGaucheRot();
    float RKi1 = 0.006;  // Gain intégral
    float RKp1 = 0.0017;  // Gain proportionnel pour A
    if (vitesseGauche < 0){
        vitesseGauche *= -1.0;
    }
    if (vitesseDroit < 0){
        vitesseDroit *= -1.0;
    }
    // Calcul des écarts entre la consigne et la mesure
    float ecartDroit = vitesseGauche - vitesseDroit;

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * RKp1;

    // Mise à jour des erreurs accumulées pour l'intégrale
    RerreurAccumuleeDroite += ecartDroit * 0.01;

    //calcul de erreur position total
    

    // Calcul des termes intégrals
    float termeIntDroit = RerreurAccumuleeDroite * RKi1;

    // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
    float correctionDroit = limiterRot(vd + termePropDroit + termeIntDroit, -1.0, 1.0);
    Serial.print("vitesse droit:");
    Serial.println(vitesseDroit);
    Serial.print("vitesse gauche:");
    Serial.println(vitesseGauche);

    // Application des corrections aux moteurs
    if (tourneDroit == 0){
        MOTOR_SetSpeed(RIGHT, correctionDroit);
    }
    if (tourneGauche == 0){
        MOTOR_SetSpeed(LEFT, vg);        
    }
    return RerreurAccumuleeDroite;
}

// Fonction qui fait tourner le robot a gauche
// a = angle de rotation en degré
void rotationGauche(float a) {
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*cirRoue)*pulseTourRoue;
    float verif = (pulseTourRoue/cirRoue)*(a*(cirCerRot/tour));
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;
    float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        RerreurAccumuleeDroite = CorrigerVitesseRot(vitesseRotationPos, vitesseRotationNeg, tourneDroite, tourneGauche, RerreurAccumuleeDroite);

        while ((pulseGauche < ((pulse-300.0) * -1.0) || pulseDroite > pulse-300.0) && (tourneDroite == 0 || tourneGauche == 0)){
            pulseGauche = ENCODER_Read(LEFT);
            pulseDroite = ENCODER_Read(RIGHT);
            if (pulseGauche < ((pulse-50) * -1.0)){
                MOTOR_SetSpeed(LEFT, 0);
                tourneGauche = 1;
            }
            if (pulseDroite > (pulse-50)){
                MOTOR_SetSpeed(RIGHT, 0);
                tourneDroite = 1;
            }
            delay(10);
        }
    }
}

// Fonction qui fait tourner le robot à droite
// a = angle de rotation en degré
void rotationDroite(float a) {
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*cirRoue)*pulseTourRoue;
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;
    float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        RerreurAccumuleeDroite = CorrigerVitesseRot(vitesseRotationNeg, vitesseRotationPos, tourneDroite, tourneGauche, RerreurAccumuleeDroite);

        while ((pulseDroite < ((pulse-300.0) * -1.0) || pulseDroite > pulse-300.0) && (tourneDroite == 0 || tourneGauche == 0)){
            pulseGauche = ENCODER_Read(LEFT);
            pulseDroite = ENCODER_Read(RIGHT);
            if (pulseDroite  < ((pulse-50) * -1.0)){
                MOTOR_SetSpeed(RIGHT, 0);
                tourneGauche = 1;
            }
            if (pulseGauche > (pulse-50)){
                MOTOR_SetSpeed(LEFT, 0);
                tourneDroite = 1;
            }
            delay(10);
        }
    }
}

//Fonction qui permet d'appaler les bonne fonction en fontion de la direction désiré
void rotationGlobal(float angle){
    if(angle > 0){
        rotationDroite(angle);
    }
    else if(angle < 0){
        rotationGauche((angle * -1.0));
    }
}

/****************************************************** FIN de la zone des fonctions***********************************************
 * Début du main */
//

/** Fonction de départ, se fait appeler une seule fois au début du programme*/

void setup() {
	BoardInit();
    Serial.begin(9600); //Communication à 9600 bits/sec
    
    delay(10);
    Serial.println("");
    Serial.println("Setup started");
    //MOTOR_SetSpeed(0,-0.05);
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
        //MOTOR_SetSpeed(1,0.1);

        float res = detectionObjet();
        Serial.print("detectObjet : ");
        Serial.println(res);

        /*
        Serial.print("Couleur détectée : ");
        Serial.println(detectColor());
        delay(500);
        Serial.print("mouvement servo ouvert ");
        SERVO_ouvert(true);
        delay(1000);
        Serial.print("mouvement servo fermé ");
        SERVO_ouvert(false);
        */
        Serial.print("U1 : ");
        Serial.println(dataSuiveurLigne[0]);
        Serial.print("U2 : ");
        Serial.println(dataSuiveurLigne[1]);
        Serial.print("U3 : ");
        Serial.println(dataSuiveurLigne[2]);
        Serial.println("loop test finished");
        delay(500);
    }
    //fin boucle de test

    Serial.println("loop finished");
    delay(DT);
}