/********
    Fichier: avancement.cpp
    Auteurs: Florent Blanchard
    Date: 26 septembre 2024
    Description: Gestion du déplacement des robots avec correction des différences de frottement.
********/

#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <avancement.h>
#include <Rotations.h>

// Paramètres du régulateur PI
float erreurAccumuleeGauche;  // Somme des erreurs accumulées pour la roue gauche
float erreurAccumuleeDroite;  // Somme des erreurs accumulées pour la roue droite
float totalpulseDroit;
float totalpulseGauche;
float Ki1 = 0.00005;  // Gain intégral
float Kp1 = 0.0002;  // Gain proportionnel
float Ki2 = 0.0015;  // Gain intégral
float Kp2 = 0.008;  // Gain proportionnel
float deltaT = 0.05;  // Intervalle de temps entre les cycles (en secondes)

// Fonction pour limiter une valeur dans une plage donnée
float limiter(float valeur, float minVal, float maxVal) {
    if (valeur > maxVal) return maxVal;
    if (valeur < minVal) return minVal;
    return valeur;
}

// Lecture de la vitesse de la roue droite
float lireVitesseDroit() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(deltaT * 1000);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 50.0;
    return vitesse;
}

// Lecture de la vitesse de la roue gauche
float lireVitesseGauche() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(deltaT * 1000);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 50.0;
    return vitesse;
}

// Fonction de régulation PI pour ajuster les vitesses des moteurs
void CorrigerVitesse(float vd, float vg, float distanceParcourue) {
    // Lire les vitesses actuelles basées sur les pulses des encodeurs
    float vitesseDroit = lireVitesseDroit();
    float vitesseGauche = lireVitesseGauche();
    float vdPulse = vd * 11.95;  // Conversion de la consigne en pulses
    float vgPulse = vg * 11.95;

    // Calcul des écarts de vitesse
    float ecartDroit = vdPulse - vitesseDroit;
    float ecartGauche = vgPulse - vitesseGauche;

    // Position réelle des encodeurs
    int pulseActuelDroit = ENCODER_Read(RIGHT);
    int pulseActuelGauche = ENCODER_Read(LEFT);

    // Calculer la position idéale basée sur la distance parcourue en cm
    int pulseIdeal = distanceParcourue * (3200 / 23.94);  // 3200 pulses par rotation, 23.94 cm par tour

    // Calcul de l'erreur de position (écart entre position réelle et idéale)
    float erreurPositionDroit = pulseIdeal - pulseActuelDroit;
    float erreurPositionGauche = pulseIdeal - pulseActuelGauche;

    // Ajouter l'erreur de position aux erreurs de vitesse
    ecartDroit += erreurPositionDroit * Kp1;  // Correction basée sur la position
    ecartGauche += erreurPositionGauche * Kp2;  // Correction basée sur la position

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * Kp1;
    float termePropGauche = ecartGauche * Kp2;

    // Mise à jour des erreurs accumulées pour l'intégrale
    erreurAccumuleeDroite += ecartDroit * deltaT;
    erreurAccumuleeGauche += ecartGauche * deltaT;

    // Calcul des termes intégrals
    float termeIntDroit = erreurAccumuleeDroite * Ki1;
    float termeIntGauche = erreurAccumuleeGauche * Ki2;

    // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
    float correctionDroit = limiter(vd + termePropDroit + termeIntDroit, -1.0, 1.0);
    float correctionGauche = limiter(vg + termePropGauche + termeIntGauche, -1.0, 1.0);

    // Debug : Affichage des corrections calculées
    Serial.print("Correction Droit: ");
    Serial.println(correctionDroit);
    Serial.print("Correction Gauche: ");
    Serial.println(correctionGauche);

    // Appliquer les corrections aux moteurs
    MOTOR_SetSpeed(RIGHT, correctionDroit);
    MOTOR_SetSpeed(LEFT, correctionGauche);
}


// Fonction pour l'accélération progressive
void accel(float vd, float vg, float distanceParcourue) {
    for (int i = 1; i <= 5; i++) {
        CorrigerVitesse(i * (vd / 5), i * (vg / 5), distanceParcourue);
        delay(50);  // Attendre un peu entre chaque étape d'accélération
    }
}

// Fonction de décélération progressive
void decel(int pulse, float vd, float vg) {
    int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

    while (pulsesRestants > 100) {
        // Calcul des pulses restants
        pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

        // Réduire la vitesse proportionnellement à la distance restante
        float facteurDecel = ((float)pulsesRestants * 5 ) / pulse;
        if (facteurDecel < 0.15) {
            facteurDecel = 0.15;  // Ne pas descendre en dessous de 10% de la vitesse
        }

        // Appliquer la réduction de la vitesse
        float vitesseDroit = vd * facteurDecel;
        float vitesseGauche = vg * facteurDecel;

        MOTOR_SetSpeed(RIGHT, vitesseDroit);
        MOTOR_SetSpeed(LEFT, vitesseGauche);
        Serial.print("vitesse droit decel:");
        Serial.println(vitesseDroit);
        Serial.print("vitesse gauche decel:");
        Serial.println(vitesseGauche);

        delay(50);  // Attendre entre chaque ajustement de la vitesse
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

// Fonction pour faire avancer le robot sur une distance donnée
float avance(float dist, float distanceParcourue) {
    float vd = 0.6;  // Vitesse désirée droite
    float vg = 0.6;  // Vitesse désirée gauche
    int ptr = 3200;  // Nombre de pulses par rotation
    float circRoue = 23.94;  // Circonférence de la roue en cm
    float pulseParCM = ptr / circRoue;  // Calcul du nombre de pulses par cm
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    int pulseArret = 10 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)

    // Réinitialisation des erreurs accumulées
    erreurAccumuleeDroite = 0;
    erreurAccumuleeGauche = 0;

    // Réinitialisation des encodeurs
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    // Accélération progressive
    accel(vd, vg, distanceParcourue);
    delay(50);

    // Avancer jusqu'à presque atteindre la distance cible
    while (((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) < (pulse - pulseArret)) {
        // Mettre à jour la distance parcourue en cm
        distanceParcourue = ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) / pulseParCM;

        // Correction des vitesses en fonction de la distance parcourue
        CorrigerVitesse(vd, vg, distanceParcourue);

        // Pause pour laisser le temps aux corrections
        delay(50);
    }

    // Décélération progressive
    decel(pulse, vd, vg);

    // Mise à jour de la position totale en pulses
    totalpulseDroit += ENCODER_Read(RIGHT);
    totalpulseGauche += ENCODER_Read(LEFT);

    // Debug : Affichage des pulses et de la position
    Serial.print("Total Pulses Droit: ");
    Serial.println(totalpulseDroit);
    Serial.print("Total Pulses Gauche: ");
    Serial.println(totalpulseGauche);
    Serial.print("Nombre de pulses calculé : ");
    Serial.println(pulse);
    return distanceParcourue;
}
