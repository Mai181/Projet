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
float vn = -0.25;  // vitesse négative du moteur 
float vp = 0.25;  // vitesse positive du moteur
// vitesse a tester avec le robot

// Fonction qui fait tourner le robot a gauche
// a = angle de rotation
void rotationGauche(int a) {
    float pulse = cirCerRot/((tour/a)*cirRoue)*pulseTourRoue;
    while(ENCODER_Read(LEFT) != pulse) {
        MOTOR_SetSpeed(LEFT, vn);
        MOTOR_SetSpeed(RIGHT, vp); 
        
    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    //manque quelle que chose pour arreter de tourner ( vitesse = 0 )
    //idealement faudrait des rampe d'acceleration et deceleration pour combatre l'inertie du robot en rotation
    /*si manque de pression apres les tests il pourrait avoir un reajustement de la position selon les valeur attendu 
    des encodeur avec le deplacement*/
}

// Fonction qui fait tourner le robot a droite
// a = angle de rotation
void rotationDroite(int a) {
    float pulse = cirCerRot/((tour/a)*cirRoue)*pulseTourRoue;
    while(ENCODER_Read(LEFT) != pulse) {
        MOTOR_SetSpeed(LEFT, vp);
        MOTOR_SetSpeed(RIGHT, vn); 

    }
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    //meme chose qu'en haut
}
