/********
    Fichier: main.cpp
    Auteurs: P-19
    Date: 26 septembre 2024
    Description: code principal des robots
********/

#include <Arduino.h>
#include <librobus.h>
#include <Rotations.h>
#include <avancement.h>
#include <calculPosition.h>
#include <detectionSifflet.h>
#include <math.h>

const int dt=50;//différence de temps
bool obstacleAvant, obstacleDroit, obstacleGauche; //Indique s'il y a un obstacle dans la direction
bool capteurAvant, capteurDroit; //État des capteurs
bool ligneDepart, ligneFin; //Indique que le robot est a la ligne départ ou de fin
bool ruban; //Indique qu'il y a un ruban a cette rangé
int direction = 0; //orientation du robot dans l'espace
int range = 25; //postion en Y (au départ à 25cm)
int colonne =75; //positon en X (au départ à 75cm)
bool depart = 0; //signal du siflet detecté
void setup() {
	BoardInit();
  Serial.begin(9600); //Communication à 9600 bits/sec
  setDt(dt);
  setEncochesParCentimetre();
}

void loop() {
  //Recherche du sifflet

  for(int i = 0; i < 1; i++) {
    avancement(50);
  }


  //Recherche du sufflet

  bool siffletActive = false;
  while(siffletActive == false){
    siffletActive = detectionSifflet();
  }

  //Ajustement de la variable de direction
  if(direction == 360){ //si la direction est de 360 degrés c'est equivalent à 0
    direction = 0;
  }

  if(direction < 0){  //si la direction est négative, on la remet sur 360
  direction = direction + 360;
  }

  //Calcul de la position des rubans
  if((range % 100) >= 40){
    ruban = 1;
  }
  else {
    ruban = 0;
  }

  //Calcul du depart du parcours
  if(range == 25){
    ligneDepart = 1;
  }
  else{
    ligneDepart = 0;
  }

  //Calcul de la Fin du parcours
  if(range == 525){
    ligneFin = 1;
  }
  else {
    ligneFin = 0;
  }

  
  //Calcul des obstacle à la droite du robot
  if(capteurDroit == 1 || (colonne == 125 && direction == 0) || (ligneDepart == 1 && direction == 90) || ruban == 1){
    obstacleDroit = 1;
  }
  else {
    obstacleDroit = 0;
  }

  //Calcul des obstacle à la gauche du robot
  if((colonne == 25 && direction == 0) || (ligneDepart == 1 && direction == 270) || ruban == 1){
    obstacleGauche = 1;
  }
  else {
    obstacleGauche = 0;
  }

  //Calcul des obstacle à l'avant du robot
  if(capteurAvant == 1 || (ligneDepart == 1 && direction == 180) || (colonne == 125 && direction == 90) 
    || (colonne == 25 && direction == 270) || (ruban == 1 &&(direction == 90 || direction == 270))){
    obstacleAvant = 1;
  }
  else{
    obstacleAvant = 0;
  }

  //Déplacement vers l'avant
  if(obstacleAvant == 0){
    //avancerPID(50);
    if(direction == 0){
      range += 50;
    }
    else if (direction == 90)
    {
      colonne += 50;
    }
    else if (direction == 180)
    {
      range -= 50;
    }
    else if (direction == 270)
    {
      colonne -= 50;
    }
  }

  //Tourner vers la droite
  if(obstacleDroit == 0 && obstacleAvant == 1){
    rotationDroite(90);
    direction += 90;
  }

  //Tourner vers la gauche
  if(obstacleGauche == 0 && obstacleDroit == 1 && obstacleAvant == 1){
    rotationGauche(90);
    direction -= 90;
  }

  //Si il est entouré d'obstacle il tourne à droite jusqu'a trouver une sortie
  if(obstacleAvant == 1 && obstacleDroit == 1 && obstacleGauche == 1){
    rotationDroite(90);
    direction += 90;
  }
  ENCODER_Reset(LEFT);
  ENCODER_Reset(RIGHT);
  delay(dt);
}

void setPosition(int rangeTemp, int colonneTemp){
  range=rangeTemp;
  colonne=colonneTemp;
}