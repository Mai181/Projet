/********
    Fichier: Rotations
    Auteurs: Maina Clermont 
    Date: 26 septembre 2024
    Description: rotation des robots
********/

#include <librobus.h>
#include <Rotations.h>

float cirCerRot = 60.318579;  // circonferenceCercleRotation
float cirRoue = 23.94;  // circonferenceRoue
float tour = 360.0;
float pulseTourRoue = 3200.0; 
float vn = -0.20;  // vitesse négative du moteur 
float vp = 0.20;  // vitesse positive du moteur
// vitesse a tester avec le robot

// Fonction qui fait tourner le robot a gauche
// a = angle de rotation en degré
void rotationGauche(float a) {
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
        if (pulseGauche > (pulse * -1.0)){
            MOTOR_SetSpeed(LEFT, vn);
        }
        else{
            MOTOR_SetSpeed(LEFT, 0);
            tourneGauche = 1;
        }
        if (pulseDroite < pulse){
            MOTOR_SetSpeed(RIGHT, vp);
        }
        else{
            MOTOR_SetSpeed(RIGHT, 0);
            tourneDroite = 1;
        }
        delay(10);
    }

//    Serial.println(ENCODER_Read(LEFT));
//    Serial.println(ENCODER_Read(RIGHT));
}


void rotationDroite(float a) {
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    float pulse = cirCerRot/((tour/a)*cirRoue)*pulseTourRoue;
    int tourneGauche =0;
    int tourneDroite =0;
    float pulseGauche;
    float pulseDroite;

    while (tourneGauche == 0 || tourneDroite == 0) {
        pulseGauche = ENCODER_Read(LEFT);
        pulseDroite = ENCODER_Read(RIGHT);
        if (pulseGauche < pulse){
            MOTOR_SetSpeed(LEFT, vp);
        }
        else{
            MOTOR_SetSpeed(LEFT, 0);
            tourneGauche =1;
        }
        if (pulseDroite > (pulse * -1.0)){
            MOTOR_SetSpeed(RIGHT, vn);
        }
        else{
            MOTOR_SetSpeed(RIGHT, 0);
            tourneDroite = 1;
        }
        delay(10);
    }

//    Serial.println(ENCODER_Read(LEFT));
//    Serial.println(ENCODER_Read(RIGHT));
}
