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
float Ki = 0.0005;                   // Gain intégral
float Kp = 0.0015;                   // Gain proportionnel
float deltaT = 0.05;              // Intervalle de temps entre chaque cycle (en secondes)
/*
float Ki = 0.1;                   // Gain intégral
float Kp = 1.0;                   // Gain proportionnel
float deltaT = 0.05;              // Intervalle de temps entre chaque cycle (en secondes)
*/
float lireVitesseDroit(){
  uint8_t posInit = ENCODER_Read(RIGHT);
  delay(deltaT*1000);
  float vitesse;
  vitesse = (ENCODER_Read(RIGHT) - posInit)/50; //calcul de la distance parcouru en 50ms afin de trouver la vitesse instanté
  return vitesse;
}

float lireVitesseGauche(){
  uint8_t posInit = ENCODER_Read(LEFT);
  delay(deltaT*1000);
  float vitesse;
  vitesse = (ENCODER_Read(LEFT) - posInit)/50; //calcul de la distance parcouru en 50ms afin de trouver la vitesse instanté
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
  for(int i = 1; i < 5; i ++){
    CorrigerVitesse((i * (vd/5)), (i * (vg/5)));
    delay(50);
  }
}

void decel(float vd, float vg)
{
  for(int i = 10; i > 2; i --){
    MOTOR_SetSpeed(LEFT, (i * (vg/10)));
    MOTOR_SetSpeed(RIGHT, (i * (vd/10)));
    delay(50);
  }
}

void stop()
{
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
}

void avance(float dist)
{
  float vd = 0.6; //vitesse desiré droite
  float vg = 0.6; //vitesse desiré gauche
  int ptr = 3200; //pulse par rotation
  float circRoue = 23.94; //circonférence de la roue
  float pulseParCM = ptr/circRoue; //calcule le nombre de pulse par CM
  int pulse = dist * pulseParCM; //calcul le nombre de pulse a faire pour la distance donné
  int pulseArret = 10 * pulseParCM; //distance necesaire pour arrêter en pulse

  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
  /*depart donc accélération*/
  accel(vd, vg);
  delay(50);
  while (((ENCODER_Read(RIGHT)+ENCODER_Read(LEFT))/2) < (pulse - pulseArret)){
    CorrigerVitesse(vd, vg);
  }

  if (((ENCODER_Read(RIGHT)+ENCODER_Read(LEFT))/2) < pulse){
    for(int i = 5; i > 1; i --){
      MOTOR_SetSpeed(LEFT, (i * (vg/5)));
      MOTOR_SetSpeed(RIGHT, (i * (vd/5)));
      delay(50);
      }
    int x1 = 0;
    int x2 = 0;
    for(int i = 0 ; i < 5000; i++){
      if (ENCODER_Read(RIGHT) >= pulse){
        MOTOR_SetSpeed(RIGHT, 0);
        x1 = 1;
      }
      if (ENCODER_Read(LEFT) >= pulse){
        MOTOR_SetSpeed(LEFT, 0);
        x2 = 1;
      }
      if (x1 == 1 && x2 == 1){
        break;
      }
      delay(20);
      }
  }

  Serial.print("Encodeur: ");
  Serial.println(((ENCODER_Read(RIGHT)+ENCODER_Read(LEFT))/2));
  Serial.println(ENCODER_Read(RIGHT));
  Serial.println(ENCODER_Read(LEFT));
  Serial.print("nombre calculé : ");
  Serial.println(pulse);
}
