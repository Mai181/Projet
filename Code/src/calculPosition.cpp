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
const int encochesCirconference=3200;
const float diametre=7.62;
float EncochesParCentimetre;


void setDt(int dtTemp){
  dt=dtTemp;
}

void setEncochesParCentimetre(){
  EncochesParCentimetre=encochesCirconference/(diametre*PI);
}

float distanceParcourue(){
  return (float)((ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/2)/encochesCirconference;
}