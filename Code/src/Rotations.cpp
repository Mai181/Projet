/********
    Fichier: Rotations
    Auteurs: Maina Clermont 
    Date: 26 septembre 2024
    Description: rotation des robots
********/

#include <librobus.h>
#include <Rotations.h>

float cirCerRot = 114.98;  // circonferenceCercleRotation
float cirRoue = 23.94;  // circonferenceRoue
int tour = 360;
int pulseTourRoue = 3200; 
float vn = -0.5;  // vitesse négative du moteur 
float vp = 0.5;  // vitesse positive du moteur
// vitesse a tester avec le robot

// Fonction qui fait tourner le robot a gauche
// a = angle de rotation
void rotationGauche(int a) {
    float pulse = cirCerRot/((tour/a)*cirRoue)*pulseTourRoue;
    while(ENCODER_Read(LEFT) != pulse) {
        MOTOR_SetSpeed(LEFT, vn);
        MOTOR_SetSpeed(RIGHT, vp); 
    }
}

// Fonction qui fait tourner le robot a droite
// a = angle de rotation
void rotationDroite(int a) {
    float pulse = cirCerRot/((tour/a)*cirRoue)*pulseTourRoue;
    while(ENCODER_Read(LEFT) != pulse) {
        MOTOR_SetSpeed(LEFT, vp);
        MOTOR_SetSpeed(RIGHT, vn); 
    }
}
