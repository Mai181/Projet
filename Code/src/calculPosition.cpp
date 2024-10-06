/********
    Fichier: calculPosition.h
    Auteurs: Guillaume Perron Nantel perg7015
    Date: 06 octobre 2024
    Description: déclaration des fonctions de calculPosition
********/

#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <avancement.h>
#include <calculPosition.h>
int dt=50;
float distanceParEncoche;


void setDt(int dtTemp){
  dt=dtTemp;
}

void setDistanceParEncoche(int distanceParEncocheTemp){
  distanceParEncoche=distanceParEncocheTemp;
}

int distanceParcourue(){
  return 1;
}