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
#include <math.h>


/********** DÉBUT de la zone des variables et constantes */
Adafruit_TCS34725 colorSensor;
/** nbr of tests */
int numTest = 5; 

/** Données collectés du suiveur de ligne */
int dataSuiveurLigne[3];
/** Tableau mémorisant la position des objets */
int mapObjet[360];

/** Délai en ms entre chaque itération du loop */
const int DT=50;
/** Boucle de débug */
const bool DEBUGAGE = true;

float tour = 360.0;
/** Pulse par tour de roue */
float pulseTourRoue = 3200.0; 
/** Nombre de pulses par rotation */
int ptr = 3200;  
/** Circonférence de la roue en cm */
float circRoue = 23.94;  
/** Circonference cercle rotation */ 
float cirCerRot = 60.056;   
/** Calcul du nombre de pulses par cm */
float pulseParCM = ptr / circRoue;

/** Vitesse négative du moteur */
float vitesseRotationNeg = -0.15;  
/** Vitesse positive du moteur */
float vitesseRotationPos = 0.15;  

/** Direction actuel du robot dans l'espace (face à la zone rouge étant 0 degré) */
float direction; 
/** Booléen pour la détection du sifflet */
bool siffletActive = false;
/** Longueur d'un tape (du milieu à une zone de couleur) */
float distLigne = 90.0;                                               //***À changer selon les pinces!!!!
/** Distance entre le robot et un objet */
float distObj;
/** Calcul du nombre de pulses par degré */
float pulseParDeg = (cirCerRot/360)*pulseParCM;

/********** FIN de la zone des variables et constantes
 
 * Début de la zone des fonctions */
                         /************************* Fonctions pour couleurs, servomoteurs et lignes **************************/
/** Fonction détecte couleur et retourne couleur détectée ou 
 *  erreur selon s'il y a un erreur ou selon la couleur détectée
 * 
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
/*   
    Serial.println("Red");
    Serial.println(redAvg);
    Serial.println("green");
    Serial.println(greenAvg);
    Serial.println("blue");
    Serial.println(blueAvg);
    Serial.println("clear");
    Serial.println(clear);
*/

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

/** Lorsque la fonction est appelée, soit que les servomoteurs sont fermés (45) ou ouverts (135)
 *  
 * @param ouvert: true = ouverts, false = fermés (bool)
*/
void SERVO_ouvert(bool ouvert) {
    
    uint8_t servoAngle = 45;	// angle servomoteur lorsque fermé
        if (ouvert){
            servoAngle = 135;   // angle servomoteur lorsque ouvert
        }

    SERVO_SetAngle(0, servoAngle);  // Servomoteur gauche
    SERVO_SetAngle(1, 180 - servoAngle); // Servomoteur droit
}

/** Fonction initialise servomoteurs
*/
void INIT_servos(){
    SERVO_Enable(0);
    SERVO_Enable(1);
    SERVO_ouvert(true);
}

/** Fonction de lecture du capteur de ligne
 *  Écrit un tableau des retour des capteurs : 0=noir, 1=blanc
*/
void detecteurligne(){
/**
 */
    int valeurU3=analogRead(A8);
    int valeurU2=analogRead(A9);
    int valeurU1=analogRead(A10);

    if (valeurU1 > 45){
        dataSuiveurLigne[0] = 1;
    }
    else{
        dataSuiveurLigne[0] = 0;
    }

    if (valeurU2 > 45){
        dataSuiveurLigne[1] = 1;
    }
    else{
        dataSuiveurLigne[1] = 0;
    }

    if (valeurU3 > 45){
        dataSuiveurLigne[2] = 1;
    }
    else{
        dataSuiveurLigne[2] = 0;
    }
}

/** Fonction direction en fonction de la couleur (en degré)
 *  
 *  @param c: 0 = Aucun, 1 = Rouge, 2 = Jaune, 3 = vert et 4 = bleu (int)
 *  Si la fonction est appelée avec 0, la fonction détecte la couleur sur laquelle 
 *  est le robot présentement et assigne la bonne direction. Si la fonction est 
 *  appelée avec 1 et plus, la fonction assigne la direction en fonction de la couleur voulue
 *  
 *  @return direction : 0.0, 90.0, 180.0 ou 270.0 (float)
 *  (float et non int pour ne pas avoir d'erreur avec la fonction deplacement)
*/
float directionCouleur(int c){
    char current = detectColor();
    if (c == 0) {
        while (current == 'E'){
            current = detectColor();
        }
    }
    if (current == 'R' || c == 1) {
        return 180.0;
    }
    else if (current == 'J' || c == 2){
        return 90.0;
    }
    else if (current == 'B' || c == 3){
        return 0.0;
    }
    else {
        return 270.0;
    }
}

/** Fonction pour détecter le signal de départ de 5kHz 
 *  
 *  @return bool si le sifflet est détecter (true) ou non (false)
*/
bool detectionSifflet(){

    bool sifflet = false; //par défaut, il n'y a pas de sifflet

    int bruitAmbiant = analogRead(A14); //l'information lue à la pin 10 se trouve entre 0 et 1023
    float voltageBruitAmbiant = bruitAmbiant * (5.0 / 1023.0); //produit croisé pour ramener sur 5V (l'alim du circuit)
    //Serial.println(voltageBruitAmbiant);

    int cinqkH = analogRead(A15);
    float voltage5kH = cinqkH * (5.0 / 1023.0);
    //Serial.println(voltage5kH);

    if (voltage5kH > voltageBruitAmbiant){
        sifflet = true;
    }

    return sifflet;
}

                            /************************* Fonctions pour les déplacements **************************/
                            
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

/** Lecture de la vitesse de la roue droite en ligne droite
 *  
 *  @return vitesse (float)
*/
float lireVitesseDroit() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(50);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 50.0;
    return vitesse;
}

/** Lecture de la vitesse de la roue gauche en ligne droite
 *  
 *  @return vitesse (float)
*/
float lireVitesseGauche() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(50);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 50.0;
    return vitesse;
}

/** Fonction de régulation PI pour ajuster les vitesses des moteurs
 *  
 *  @param vd: vitesse roue droite (float)
 *  @param vg: vitesse roue gauche (float)
 *  @param erreurAccumuleeDroite: erreur accumulée roue droite (float)
 *  
 *  @return erreur accumulée roue droite (float)
*/
float CorrigerVitesse(float vd, float vg, float erreurAccumuleeDroite) {
    //Parametre du PI
    float Ki1 = 0.0006;  // Gain intégral pour B elever 200 pulse pour la fin
    float Kp1 = 0.0012;  // Gain proportionnel pour B
    float totalpulseDroit = ENCODER_Read(RIGHT);
    float totalpulseGauche = ENCODER_Read(LEFT);
    float ecartDroit;
    float correctionDroit;

    if(vd > 0){
        // Calcul des écarts entre la consigne et la mesure
        ecartDroit = totalpulseGauche - totalpulseDroit;
    }
    else{
        ecartDroit = totalpulseDroit - totalpulseGauche;
    }

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * Kp1;

    // Mise à jour des erreurs accumulées pour l'intégrale
    erreurAccumuleeDroite += ecartDroit * 0.05;

    // Calcul des termes intégrals
    float termeIntDroit = erreurAccumuleeDroite * Ki1;
    if(vd > 0){
        // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
        correctionDroit = limiter(vd + termePropDroit + termeIntDroit, -1.0, 1.0);
    }
    else{
        // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
        correctionDroit = limiter(vd - termePropDroit - termeIntDroit, -1.0, 1.0);
    }

    // Application des corrections aux moteurs
    MOTOR_SetSpeed(RIGHT, correctionDroit);
    MOTOR_SetSpeed(LEFT, vg);
    Serial.println(correctionDroit);

    return erreurAccumuleeDroite;
}

/** Fonction pour l'accélération progressive
 *  
 *  @param vd: vitesse roue droite (float)
 *  @param vg: vitesse roue gauche (float)
 *  @param erreurAccumuleeDroite: erreur accumulée roue droite (float)
 *  
 *  @return erreur accumulée roue droite (float)
*/
float accel(float vd, float vg, float erreurAccumuleeDroite) {
    for (int i = 1; i <= 10; i++) {
        erreurAccumuleeDroite = CorrigerVitesse(i * (vd / 10), i * (vg / 10), erreurAccumuleeDroite);
        delay(75);  // Attendre un peu entre chaque étape d'accélération
    }
    return erreurAccumuleeDroite;
}

/** Fonction pour arrêter les moteurs */
void stop() {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

/** Fonction de décélération progressive
 *  
 *  @param pulse: nbr pulse (int)
 *  @param erreurAccumuleeDroite: erreur accumulée roue droite (float)
 *  
 *  @return erreur accumulée roue droite (float)
*/
float decel(int pulse, float erreurAccumuleeDroite) {
    float topSpeedDroit = 11.08;
    float topSpeedGauche = 10.90;
    float ReadedSpeedDroit = lireVitesseDroit();
    float ReadedSpeedGauche = lireVitesseGauche();
    float RealSpeedDroit = ReadedSpeedDroit/topSpeedDroit;
    float RealSpeedGauche = ReadedSpeedGauche/topSpeedGauche;

    int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

    float i = 9.0;

    if(pulse > 0){
        while (pulsesRestants > 20) {
            if (i >= 0){
            erreurAccumuleeDroite = CorrigerVitesse(i * (RealSpeedDroit / 10.0), i * (RealSpeedGauche / 10.0), erreurAccumuleeDroite);
            delay(75);  // Attendre un peu entre chaque étape d'accélération
            i--;
            }
            pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
        }
    }
    else{
        while (pulsesRestants < -20) {
            if (i >= 0){
            erreurAccumuleeDroite = CorrigerVitesse(i * (RealSpeedDroit / 10.0), i * (RealSpeedGauche / 10.0), erreurAccumuleeDroite);
            delay(75);  // Attendre un peu entre chaque étape d'accélération
            i--;
            }
            pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
        }
    }

    stop();
    return erreurAccumuleeDroite;
}

/** Fonction pour faire avancer le robot sur une distance donnée
 * 
 *  @param dist: distance (float)
*/
void deplacement(float dist) {
    float vd = 0.6;  // Vitesse désirée droite
    float vg = 0.6;  // Vitesse désirée gauche
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    int pulseArret = 20 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)
    float erreurAccumuleeDroite =0; //erreur accumuler de la roue droite pi le I du PI

    // Réinitialisation des encodeurs
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    if(dist > 0){
        // Accélération progressive
        erreurAccumuleeDroite = accel(vd, vg, erreurAccumuleeDroite);
        delay(50);
        // Avancer jusqu'à presque atteindre la distance cible
        while (((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) < (pulse - pulseArret)) {

            // Correction des vitesses en fonction de la distance parcourue
            erreurAccumuleeDroite = CorrigerVitesse(vd, vg, erreurAccumuleeDroite);

            // Pause pour laisser le temps aux corrections
            delay(50);
        }
    }
    else{
        // Accélération progressive
        erreurAccumuleeDroite = accel((vd*-1), (vg*-1), erreurAccumuleeDroite);
        delay(50);
                // Avancer jusqu'à presque atteindre la distance cible
        while (((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) > (pulse + pulseArret)) {

            // Correction des vitesses en fonction de la distance parcourue
            erreurAccumuleeDroite = CorrigerVitesse((vd*-1), (vg*-1), erreurAccumuleeDroite);

            // Pause pour laisser le temps aux corrections
            delay(50);
        }
    }

    // Décélération progressive
    erreurAccumuleeDroite = decel(pulse, erreurAccumuleeDroite);
}

/** Fonction qui permet au robot de suivre une ligne sur une distance donnée
 *  
 *  @param dist: distance (float)
*/
void suivreligne(float dist){
    float vitesseBaseDroit = -0.4;  // Vitesse désirée droite
    float vitesseBaseGauche = -0.4;  // Vitesse désirée gauche
    float vitesseDroit;
    float vitesseGauche;
    float correction = 0.005;
    float erreurAccumuleeDroite =0; //erreur accumuler de la roue droite pi le I du PI
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    int pulseArret = 20 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);

    detecteurligne();
    //determine qu'il y a une ligne a suivre
    int perteLigne = 0; //utilisé comme compteur, is apres Xsec il ne retrouve pas la ligne il arrête

    if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 1 && dataSuiveurLigne[2] == 0){

        erreurAccumuleeDroite = accel(vitesseBaseDroit, vitesseBaseGauche, erreurAccumuleeDroite);
        int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

        while(pulsesRestants > pulseArret && perteLigne < 15){

            detecteurligne();
            if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[2] == 0){
                perteLigne++;
            }
            else{
                perteLigne = 0;
            }
            
            //détermine qu'il est trop à gauche
            if(dataSuiveurLigne[0] == 1 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[2] == 0){
                vitesseDroit = vitesseBaseDroit - (correction * -1);
                vitesseGauche = vitesseBaseGauche - (correction * 1);
            }
            //détermine qu'il est trop à droite
            else if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[2] == 1){
                vitesseDroit = vitesseBaseDroit - (correction * 1);
                vitesseGauche = vitesseBaseGauche - (correction * -1);
            }
            else{
                erreurAccumuleeDroite = CorrigerVitesse(vitesseBaseDroit, vitesseBaseGauche, erreurAccumuleeDroite);
            }
            MOTOR_SetSpeed(LEFT, vitesseGauche);
            MOTOR_SetSpeed(RIGHT, vitesseDroit);
            pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
            delay(100);
        }

        erreurAccumuleeDroite = decel(pulse, erreurAccumuleeDroite);
    }
}

/** Fonction de positionement du robot au centre lors du départ 
 * (le robot est considéré comme étant dos au centre)*/
void retourCentre(){
    int dist = 100; //arbitraire, le centre est a moin de 100cm d'une zone
    float distBordCentre = 20; //Distance à parcourire entre le bord de la zone noir du centre et le centre de la zone noir par rapport au centre de l'essieux
    float vd = -0.6;  // Vitesse désirée droite
    float vg = -0.6;  // Vitesse désirée gauche
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    float erreurAccumuleeDroite =0; //erreur accumuler de la roue droite pi le I du PI

    erreurAccumuleeDroite = accel(vd, vg, erreurAccumuleeDroite);

    while(dataSuiveurLigne[0] == 0 || dataSuiveurLigne[1] == 0 || dataSuiveurLigne[2] == 0){
        
        // Correction des vitesses en fonction de la distance parcourue
        erreurAccumuleeDroite = CorrigerVitesse(vd, vg, erreurAccumuleeDroite);

        // Pause pour laisser le temps aux corrections
        delay(50);
    }
    pulse = (distBordCentre * pulseParCM) + ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
    erreurAccumuleeDroite = decel(pulse, erreurAccumuleeDroite);


}

                            /************************* Fonctions pour la rotation **************************/

/** Lecture de la vitesse de la roue droite en rotation
 *  
 *  @return vitesse (float)
*/
float lireVitesseDroitRot() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(10);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 10.0;
    return vitesse;
}

/** Lecture de la vitesse de la roue gauche en rotation
 *  
 *  @return vitesse (float)
*/
float lireVitesseGaucheRot() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(10);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 10.0;
    return vitesse;
}

/** Fonction de régulation PI pour ajuster les vitesses des moteurs
 *  
 *  @param vd: vitesse droite (float)
 *  @param vg: vitesse gauche (float) 
 *  @param tourneDroit: si 0 = pas fini tourner, si 1 = fini tourner (int)
 *  @param tourneGauche: si 0 = pas fini tourner, si 1 = fini tourner (int)
 *  @param RerreurAccumuleeDroite: erreur accumulée roue droite en rotation (float)
 *  
 *  @return erreur accumulée roue droite en rotation (float)
*/
float CorrigerVitesseRot(float vd, float vg, int tourneDroit, int tourneGauche, float RerreurAccumuleeDroite) {
    float vitesseDroit = ENCODER_Read(RIGHT);
    float vitesseGauche = ENCODER_Read(LEFT);
    float RKi1 = 0.0006;  // Gain intégral
    float RKp1 = 0.0017;  // Gain proportionnel pour A
    float ecartDroit;

    if (vitesseGauche < 0){
        vitesseGauche *= -1.0;
        ecartDroit = vitesseGauche - vitesseDroit;
    }
    if (vitesseDroit < 0){
        vitesseDroit *= -1.0;
        ecartDroit = vitesseGauche - vitesseDroit;
        ecartDroit *= (-1);
    }
    // Calcul des écarts entre la consigne et la mesure
     

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * RKp1;

    // Mise à jour des erreurs accumulées pour l'intégrale
    RerreurAccumuleeDroite += ecartDroit * 0.02;

    //calcul de erreur position total
    

    // Calcul des termes intégrals
    float termeIntDroit = RerreurAccumuleeDroite * RKi1;

    // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
    float correctionDroit = limiter(vd + termePropDroit + termeIntDroit, -1.0, 1.0);

    // Application des corrections aux moteurs
    if (tourneDroit == 0){
        MOTOR_SetSpeed(RIGHT, correctionDroit);
    }
    if (tourneGauche == 0){
        MOTOR_SetSpeed(LEFT, vg);        
    }
    return RerreurAccumuleeDroite;
}

/** Fonction qui fait tourner le robot a gauche
 * 
 *  @param a: angle de rotation (float)
*/
void rotationGauche(float a) {
    // a = angle de rotation en degré
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*circRoue)*pulseTourRoue;
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;
    float RerreurAccumuleeDroite;  // Somme des erreurs accumulées pour la roue droite

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        RerreurAccumuleeDroite = CorrigerVitesseRot(vitesseRotationPos, vitesseRotationNeg, tourneDroite, tourneGauche, RerreurAccumuleeDroite);

        while ((pulseGauche < ((pulse-300.0) * -1.0) || pulseDroite > pulse-300.0) && (tourneDroite == 0 || tourneGauche == 0)){
            pulseGauche = ENCODER_Read(LEFT);
            pulseDroite = ENCODER_Read(RIGHT);
            if (pulseGauche < ((pulse) * -1.0)){
                MOTOR_SetSpeed(LEFT, 0);
                tourneGauche = 1;
            }
            if (pulseDroite > (pulse)){
                MOTOR_SetSpeed(RIGHT, 0);
                tourneDroite = 1;
            }
            delay(10);
        }
        delay(20);
    }
    direction += ((pulseDroite+(pulseGauche*-1))/2)/pulseParDeg;
}

/** Fonction qui fait tourner le robot à droite
 * 
 *  @param a: angle de rotation (float)
*/
void rotationDroite(float a) {
    // a = angle de rotation en degré
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*circRoue)*pulseTourRoue;
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;
    float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        RerreurAccumuleeDroite = CorrigerVitesseRot(vitesseRotationNeg, vitesseRotationPos, tourneDroite, tourneGauche, RerreurAccumuleeDroite);

        while ((pulseDroite < ((pulse-300.0) * -1.0) || pulseGauche > pulse-300.0) && (tourneDroite == 0 || tourneGauche == 0)){
            pulseGauche = ENCODER_Read(LEFT);
            pulseDroite = ENCODER_Read(RIGHT);
            if (pulseDroite  < ((pulse) * -1.0)){
                MOTOR_SetSpeed(RIGHT, 0);
                tourneGauche = 1;
            }
            if (pulseGauche > (pulse)){
                MOTOR_SetSpeed(LEFT, 0);
                tourneDroite = 1;
            }
            delay(10);
        }
    }
    direction += (((pulseDroite*-1)+pulseGauche)/2)/pulseParDeg;
}

/** Fonction qui permet de faire un rotation de manière globale (pas de 
 *  gauche ou droite, plutot -90 et 90 degrés)
 * 
 *  @param directionCible: direction ciblée (float)
*/
void positionnementGlobal(float directionCible){
    float angle = directionCible - direction;
    Serial.print("angle:");
    Serial.println(angle);
    if(angle > 0){
        rotationDroite(angle);
    }
    else if(angle < 0){
        rotationGauche((angle * -1.0));
    }
}

/** Fonction qui repositionne le robot sur la ligne en avant de lui */
void positionnementLigne(){
    float angle = 1.0; //angle de rotation entre chaque verification du capteur
    detecteurligne();
    if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[2] == 0){
        positionnementGlobal(direction+20.0);
    }
    if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[2] == 1){
        while(dataSuiveurLigne[0] != 0 || dataSuiveurLigne[1] != 1 || dataSuiveurLigne[2] != 0){
            positionnementGlobal(direction + angle);
            detecteurligne();
        }
    }
    else{
        while(dataSuiveurLigne[0] != 0 || dataSuiveurLigne[1] != 1 || dataSuiveurLigne[2] != 0){
            positionnementGlobal(direction - angle);
            detecteurligne();
        }
    }
    direction = direction - ((int)direction%90); //remet la direction sur le quart de cercle le plus proche (0, 90, 180, 270 ou 360)
}

                        /************************* Fonctions pour la détection d'objets **************************/


/** Donne la distance avec l'obstacle/objet devant, plus grande 
 * précision à 10cm suivi d'une imprécision grandissante en augmentant 
 * la distance et énormément grandissante en réduisant la distance
 * 
 * @return Une valeur réelle correspondant à la distance entre le 
 * capteur et l'objet
*/
float distanceObjet(){
    float res=0.0;
    int current=0;
    int test=5;
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
        delay(5/test);
    }
    Serial.print("min : ");
    Serial.println(min);
    Serial.print("max : ");
    Serial.println(max);
    //Calcul traduisant la valeur analog en cm, document avec les calculs disponible sur Teams
    return 10*(25-(sqrtf(10)*sqrtf(63*(res/((float)test))-2500)/sqrtf(res/((float)test))))-1.34445983;
}

/** Place une donnée en lien à la détection d'objet dans un tableau
 * 
 * @param donnee Donnée à enregistrer
*/
void setMemoireObjet(int donnee){
    mapObjet[(int)direction]=donnee;
}

/** Retourne le centre de l'objet le plus proche dans le sens horaire
 * 
 * @param firstValue angle de départ pour la recherche d'objet dans le sens horaire
 * 
 * @return Angle du centre de l'objet selon 0 deg, l'absence d'objet donne -1
 */
int getMemoireObjet(int firstValue){
    for(int i=0;i<90;i++){
        if(mapObjet[(firstValue+i)%360]==1){
            for(int k=i;k<90;k++){
                if(mapObjet[(firstValue+i)%360]==-1){
                    return firstValue+(i+k-1)/2;
                }
            }
        }
    }
    return -1;
}

/** Détecte s'il y a un objet devant, fait la différence entre un mur et un objet
 * 
 * @return valeur bouléenne : true=objet, false=mur
 */
bool detectionObjet(){
    float capteurDisHaut=0;
    float capteurDisBas=0;
    int test=2;
    for(int i=test;i>0;i--){
        capteurDisHaut+=analogRead(A7);
        capteurDisBas+=analogRead(A6);
        delay(0.1);
    }
    float incertitudeMultiplicative=1.1495;
    if(capteurDisBas*incertitudeMultiplicative > capteurDisHaut && capteurDisHaut*incertitudeMultiplicative > capteurDisBas){
        setMemoireObjet(1);
        return true;
    }
    setMemoireObjet(-1);
    return false;
}


/** Fonction qui permet de scanner et positionner le robot vers l'objet 
 * 
 * @return Angle global à lequel l'object à été détecté dans la zone de 90 degrés suivant l'angle initial
*/
float radar(){
    // a = angle de rotation en degré
    float a = 90;
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*circRoue)*pulseTourRoue;
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;
    float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite
    float dirInit = direction;

    while((tourneDroite == 0 || tourneGauche == 0) && (pulse > ((pulseDroite+pulseGauche)/2))){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        direction = dirInit + (((pulseDroite*-1)+pulseGauche)/2)/pulseParDeg;
        detectionObjet();
        RerreurAccumuleeDroite = CorrigerVitesseRot(vitesseRotationNeg, vitesseRotationPos, tourneDroite, tourneGauche, RerreurAccumuleeDroite);

        if(getMemoireObjet(dirInit) != -1 ){
            pulse = (((pulseDroite*-1)+pulseGauche)/2) + 300.0;
            while(tourneDroite == 0 || tourneGauche == 0){
                pulseGauche = ENCODER_Read(LEFT);
                pulseDroite = ENCODER_Read(RIGHT);
                
                if (pulseDroite  < ((pulse) * -1.0)){
                    MOTOR_SetSpeed(RIGHT, 0);
                    tourneGauche = 1;
                }
                if (pulseGauche > (pulse)){
                    MOTOR_SetSpeed(LEFT, 0);
                    tourneDroite = 1;
                }
                delay(10);
            }
        }
    }
    direction = dirInit + (((pulseDroite*-1)+pulseGauche)/2)/pulseParDeg;
    float angle = (float)getMemoireObjet(dirInit);
    return angle;
}

                                /************************* Fonction principale **************************/

/** Fonction décisionnelle pour le défi (programme principal) */
void decisions(){
    positionnementGlobal(direction);
    positionnementGlobal(radar());
    distObj = distanceObjet();
    // Double vérification de la distance si elle est supérieure à 15 cm
    if (distObj > 15.0){
        deplacement(distObj - 15.0);
        float distanceRestante = distanceObjet();
        deplacement(distanceRestante);
    }
    else {
        deplacement(distObj);
    }
    SERVO_ouvert(false);
    deplacement(distObj*-1.0);
    positionnementGlobal(direction);
    deplacement(distLigne);               
    SERVO_ouvert(true);
    deplacement(distLigne * -1.0);
}

/************************************ FIN de la zone des fonctions - Début du main **************************************/

/** Fonction de départ, se fait appeler une seule fois au début du programme*/
void setup() {
	BoardInit();
    Serial.begin(9600); //Communication à 9600 bits/sec
    delay(10);
    Serial.println("");
    Serial.println("Setup started");
    delay(100);
    Wire.begin();
    // colorSensor.ledStatus = 1;
    INIT_servos();
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(A10, INPUT);
    direction = directionCouleur(0);
    Serial.println("Setup finished");
}

/** Fonction de départ, se fait appeler à chaque fois qu'elle est terminée */
void loop(){
/*    //boucle de test : code temporaire qui peut être remplacé et effacé
    Serial.println("loop started");
    while(DEBUGAGE){
        //code temporaire qui peut être remplacé et effacé
        Serial.println("loop test started");
        
        Serial.println("loop test finished");
    }
    //fin boucle de test

    //Recherche du sifflet
    while(siffletActive == false){
        siffletActive = detectionSifflet();
        if (ROBUS_IsBumper(REAR) == 1){
            siffletActive = true;
        }
    }

    // Avance vers le milieu
    deplacement(distLigne);
    
    // Déroulement du programme principal
    for (int i = 1; i < 5; i++) {
        decisions();
        // Augmente la direction en fonction des couleurs
        direction = i + 1;
    }

    Serial.println("loop finished");
    delay(DT);
    */
   direction = 0;
   float x =0;
    while(direction < 350){
        positionnementGlobal(x+=30);
        delay(500);
        Serial.println(direction);
    }
    delay(5000);
}