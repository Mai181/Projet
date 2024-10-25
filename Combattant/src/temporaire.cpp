// fonction main 

//fonction - Avancer d'un distance données
/*reprise du code du défi du parcour*/

// Fonction pour limiter une valeur dans une plage donnée
float limiter(float valeur, float minVal, float maxVal) {
    if (valeur > maxVal) return maxVal;
    if (valeur < minVal) return minVal;
    return valeur;
}

// Lecture de la vitesse de la roue droite
float lireVitesseDroit() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(50);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 50.0;
    return vitesse;
}

// Lecture de la vitesse de la roue gauche
float lireVitesseGauche() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(50);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 50.0;
    return vitesse;
}

// Fonction de régulation PI pour ajuster les vitesses des moteurs
float CorrigerVitesse(float vd, float vg, float erreurAccumuleeDroite) {
    //Parametre du PI
    float Ki1 = 0.0006;  // Gain intégral pour B elever 200 pulse pour la fin
    float Kp1 = 0.0017;  // Gain proportionnel pour B
    float totalpulseDroit = ENCODER_Read(RIGHT);
    float totalpulseGauche = ENCODER_Read(LEFT);

    // Calcul des écarts entre la consigne et la mesure
    float ecartDroit = totalpulseGauche - totalpulseDroit;
    Serial.print("ecrat:");
    Serial.println(ecartDroit);

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * Kp1;
    Serial.print("termePropDroit:");
    Serial.println(termePropDroit);

    // Mise à jour des erreurs accumulées pour l'intégrale
    erreurAccumuleeDroite += ecartDroit * 0.05;

    // Calcul des termes intégrals
    float termeIntDroit = erreurAccumuleeDroite * Ki1;
    // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
    float correctionDroit = limiter(vd + termePropDroit + termeIntDroit, -1.0, 1.0);

    // Application des corrections aux moteurs
    MOTOR_SetSpeed(RIGHT, correctionDroit);
    MOTOR_SetSpeed(LEFT, vg);
    Serial.println(correctionDroit);

    return erreurAccumuleeDroite;
}

// Fonction pour l'accélération progressive
void accel(float vd, float vg) {
    for (int i = 1; i <= 10; i++) {
        CorrigerVitesse(i * (vd / 10), i * (vg / 10));
        delay(75);  // Attendre un peu entre chaque étape d'accélération
    }
}



// Fonction de décélération progressive
void decel(int pulse, float vd, float vg) {
    int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
    int i = 4;

    while (pulsesRestants > 200) {
          if (i >= 1){
          CorrigerVitesse(i * (vd / 5), i * (vg / 5));
          delay(50);  // Attendre un peu entre chaque étape d'accélération
          i--;
          }
          
          pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
      }
    // Assurer l'arrêt complet des moteurs
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

// Fonction pour arrêter les moteurs
void stop() {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

/*----------------------------------------------------Changement de fonction---------------------------------------------------------*/
/*Plusieur fonctions de base codées plus haut son réutilisées*/

// Fonction pour faire avancer le robot sur une distance donnée
void deplacement(float dist) {
    float vd = 0.6;  // Vitesse désirée droite
    float vg = 0.6;  // Vitesse désirée gauche
    int ptr = 3200;  // Nombre de pulses par rotation
    float circRoue = 23.94;  // Circonférence de la roue en cm
    float pulseParCM = ptr / circRoue;  // Calcul du nombre de pulses par cm
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    int pulseArret = 15 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)
    float erreurAccumuleeDroite =0; //erreur accumuler de la roue droite pi le I du PI
    float distanceParcourue = 0; // Suivi de la distance parcourue

    // Réinitialisation des encodeurs
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    // Accélération progressive
    accel(vd, vg);
    delay(50);

    // Avancer jusqu'à presque atteindre la distance cible
    while (((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) < (pulse - pulseArret)) {

        // Mettre à jour la distance parcourue en cm
        distanceParcourue = ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) / pulseParCM;

        // Correction des vitesses en fonction de la distance parcourue
        erreurAccumuleeDroite = CorrigerVitesse(vd, vg, erreurAccumuleeDroite);

        // Pause pour laisser le temps aux corrections
        delay(50);
    }

    // Décélération progressive
    decel(pulse, vd, vg);
}


/*----------------------------------------------------Changement de fonction---------------------------------------------------------*/
/*Plusieur fonctions de base codées plus haut son réutilisées*/


void suivreligne(){
    float vitesseBaseDroit = 0.6;  // Vitesse désirée droite
    float vitesseBaseGauche = 0.6;  // Vitesse désirée gauche
    float vitesseDroit;
    float vitesseGauche;
    float correction = 0.05;
    float erreurAccumuleeDroite =0; //erreur accumuler de la roue droite pi le I du PI

    detecteurligne()
    //determine qu'il y a une ligne a suivre
    if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 1 && dataSuiveurLigne[0] == 0){
        accel(vd, vg);

        while(dataSuiveurLigne[0] == 1 && dataSuiveurLigne[1] == 1 && dataSuiveurLigne[0] == 1){
            detecteurligne()
            //détermine qu'il est trop à gauche
            if(dataSuiveurLigne[0] == 1 && dataSuiveurLigne[1] == 1 && dataSuiveurLigne[0] == 0){
                vitesseDroit = vitesseBaseDroit + (correction * 1);
                vitesseGauche = vitesseBaseGauche + (correction * -1);
            }
            //détermine qu'il est trop à droite
            else if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 1 && dataSuiveurLigne[0] == 1){
                vitesseDroit = vitesseBaseDroit + (correction * -1);
                vitesseGauche = vitesseBaseGauche + (correction * 1);
            }
            //détermine qu'il est trop à gauche
            else if(dataSuiveurLigne[0] == 1 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[0] == 0){
                vitesseDroit = vitesseBaseDroit + (correction * 2);
                vitesseGauche = vitesseBaseGauche + (correction * -2);
            }
            //détermine qu'il est trop à droite
            else if(dataSuiveurLigne[0] == 0 && dataSuiveurLigne[1] == 0 && dataSuiveurLigne[0] == 1){
                vitesseDroit = vitesseBaseDroit + (correction * -2);
                vitesseGauche = vitesseBaseGauche + (correction * 2);
            }
            else{
                erreurAccumuleeDroite = CorrigerVitesse(vitesseBaseDroit, vitesseBaseGauche, erreurAccumuleeDroite);
            }
            MOTOR_SetSpeed(LEFT, vitesseGauche);
            MOTOR_SetSpeed(RIGHT, vitesseDroit);
            delay(50);
        }
        decel(32000, vitesseDroit, vitesseGauche);
    }
}

/*----------------------------------------------------Changement de fonction---------------------------------------------------------*/
/*AUCUNE fonction de base codé plus haut sont réutilisées*/
/********
    Fichier: Rotations
    Auteurs: P-19
    Date: 26 septembre 2024
    Description: code des rotation des robots
********/

#include <librobus.h>
#include <Rotations.h>
/*-------------------------------!!Ces variables doivent être global!!--------------------------------*/

float cirCerRot = 58.0;  // circonferenceCercleRotation
float cirRoue = 23.94;  // circonferenceRoue
float tour = 360.0;
float pulseTourRoue = 3200.0; 
float vitesseRotationNeg = -0.17;  // vitesse négative du moteur 
float vitesseRotationPos = 0.17;  // vitesse positive du moteur
// vitesse a tester avec le robot

// Paramètres du régulateur PI
float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite

//float RKi1 = 0.0006;  // Gain intégral pour B elever 200 pulse pour la fin
//float RKp1 = 0.0017;  // Gain proportionnel pour B


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