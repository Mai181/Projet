/********
    Fichier: Rotations
    Auteurs: Maina Clermont 
    Date: 26 septembre 2024
    Description: rotation des robots
********/

#include <librobus.h>
#include <Rotations.h>

float cirCerRot = 58.0;  // circonferenceCercleRotation
float cirRoue = 23.94;  // circonferenceRoue
float tour = 360.0;
float pulseTourRoue = 3200.0; 
float vn = -0.20;  // vitesse négative du moteur 
float vp = 0.20;  // vitesse positive du moteur
// vitesse a tester avec le robot

// Paramètres du régulateur PI
float RerreurAccumuleeGauche = 0;  // Somme des erreurs accumulées pour la roue gauche
float RerreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite
float RdeltaT = 0.01;  // Intervalle de temps entre les cycles (en secondes)
float RKi1 = 0.005;  // Gain intégral
float RKp1 = 0.0015;  // Gain proportionnel pour A


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
void CorrigerVitesseRot(float vd, float vg, int tourneDroit, int tourneGauche) {
    float vitesseDroit = lireVitesseDroitRot();
    float vitesseGauche = lireVitesseGaucheRot();
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
    RerreurAccumuleeDroite += ecartDroit * RdeltaT;

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
}

// Fonction qui fait tourner le robot a gauche
// a = angle de rotation en degré
void rotationGauche(float a) {
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*cirRoue)*pulseTourRoue;
    float verif = (pulseTourRoue/cirRoue)*(a*(cirCerRot/tour));
    Serial.print("Verification:     ");
    Serial.println(verif);
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        CorrigerVitesseRot(vp, vn, tourneDroite, tourneGauche);

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
    Serial.print("Gauche:");
    Serial.println(ENCODER_Read(LEFT));
    Serial.print("Droit:");
    Serial.println(ENCODER_Read(RIGHT));
    Serial.println(pulse);
    }
}


void rotationDroite(float a) {
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);
    float pulse = (cirCerRot)/((tour/a)*cirRoue)*pulseTourRoue;
    float pulseGauche;
    float pulseDroite;
    int tourneGauche =0;
    int tourneDroite =0;

    while(tourneDroite == 0 || tourneGauche == 0){
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        CorrigerVitesseRot(vn, vp, tourneDroite, tourneGauche);

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
    Serial.print("Gauche:");
    Serial.println(ENCODER_Read(LEFT));
    Serial.print("Droit:");
    Serial.println(ENCODER_Read(RIGHT));
    Serial.println(pulse);
    }
}