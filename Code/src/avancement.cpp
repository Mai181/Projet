/********
    Fichier: avancement.cpp
    Auteurs: Florent Blanchard
    Date: 26 septembre 2024
    Description: avancement des robots
********/

#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <avancement.h>
#include <Rotations.h>

float erreurAccumuleeGauche = 0;  // Somme des erreurs pour la roue gauche
float erreurAccumuleeDroite = 0;  // Somme des erreurs pour la roue droite
float Ki = 0.1;                   // Gain intégral
float Kp = 1.0;                   // Gain proportionnel
float deltaT = 0.05;               // Intervalle de temps entre chaque cycle (en secondes)

float lireVitesseDroit(){
  unit8_t posInit = ENCODER_Read(RIGHT);
  delay(deltaT*1000);
  float vitesse;
  vitesse = (ENCODER_Read(RIGHT) - posInit)/50; //clacul de la distance parcouru en 50ms afin de trouver la vitesse instanté
  return vitesse;
}

float lireVitesseGauche(){
  unit8_t posInit = ENCODER_Read(LEFT);
  delay(deltaT*1000);
  float vitesse;
  vitesse = (ENCODER_Read(LEFT) - posInit)/50; //clacul de la distance parcouru en 50ms afin de trouver la vitesse instanté
  return vitesse;
}

void CorrigerVitesse(float vd, float vg){
  float vitesseDroit = lireVitesseDroit();
  float vitesseGauche = lireVitesseGauche();
  float vdPulse = vd * 11.95; //vitesse voulu en pulse avec regle de 3
  float vgPulse = vg * 11.95; //vitesse voulu en pulse avec regle de 3

/*calcule de erreur ou ecart entre la consigne et la mesure*/
  float ecartGauche = vdPulse - vitesseDroit;
  float ecartDroit = vgPulse - vitesseGauche;

/*calcul du terme proportionnel de chaque côté*/
  float termePropDroit = ecartDroit * Kp;
  float termePropGauche = ecartGauche * Kp;

/*calcul erreurs accumulées*/
  erreurAccumuleeDroite += (erreurAccumuleeDroite + ecartDroit * deltaT);
  erreurAccumuleeGauche += (erreurAccumuleeGauche + ecartGauche * deltaT);

/*calcul de l'integrale*/
  float termeIntDroit = erreurAccumuleeDroite * Ki;
  float termeIntGauche = erreurAccumuleeGauche * Ki;

/*calcul de la correction du PI*/
  MOTOR_SetSpeed(RIGHT, (vd + termePropDroit + termeIntDroit));
  MOTOR_SetSpeed(LEFT, (vg + termePropGauche + termeIntGauche));
}

void accel(float vd, float vg)
{
  MOTOR_SetSpeed(LEFT,vg/2);
  MOTOR_SetSpeed(RIGHT,vd/2);
  delay(500);
  MOTOR_SetSpeed(LEFT,vg);
  MOTOR_SetSpeed(RIGHT,vd); 
}

void decel(float vd, float vg)
{
  MOTOR_SetSpeed(LEFT,vg/2);
  MOTOR_SetSpeed(RIGHT,vd/2);
  delay(500);
  MOTOR_SetSpeed(LEFT,vg/4);
  MOTOR_SetSpeed(RIGHT,vd/4);
}


void deplacement(int dist)
{
  float vd = 0.8; //vitesse desiré droite
  float vg = 0.8; //vitesse desiré gauche
  int ptr = 3200; //pulse par rotation
  float circRoue = 23.94; //circonférence de la roue
  float pulseParCM = ptr/circRoue; //calcule le nombre de pulse par CM
  int pulse = dist * pulseParCM; //calcul le nombre de pulse a faire pour la distance donné
  int pulseArret = 5 * pulseParCM; //distance necesaire pour arrêter en pulse

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
  /*depart donc accélération*/
  accel(vd, vg);
  delay(50);
  while (((ENCODER_Read(RIGHT)+ENCODER_Read(LEFT))/2) < (pulse - pulseArret)){
    
  }
  while (((ENCODER_Read(RIGHT)+ENCODER_Read(LEFT))/2) < pulse){
    decel(vd, vg)
  }
  stop()
}
