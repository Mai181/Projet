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
float dist=50.0;  //la distance entre deux centre de carre
int pulseroue=3200;
float pulseDistance(float dist,int ptr, float cirRoue) //dis= distance a parcourire en cm (du centre d'un carre a un autre est de 50cm)
{
  int pulse=(ptr/cirRoue)*dist;
  return pulse;   //le nombre de pulse pour parcourire la distance voulue.
}

void accel(float vd, float vg)
{
  MOTOR_SetSpeed(LEFT,vg/2);
  MOTOR_SetSpeed(RIGHT,vd/2);
  delay(500);
  MOTOR_SetSpeed(LEFT,vg);
  MOTOR_SetSpeed(RIGHT,vd); 
}

void avance(float vd,float vg)
{
  MOTOR_SetSpeed(LEFT,vg);
  MOTOR_SetSpeed(RIGHT,vd);
}

void stop(float vd, float vg)
{
  MOTOR_SetSpeed(LEFT,vg/2);
  MOTOR_SetSpeed(RIGHT,vd/2);
  delay(500);
  MOTOR_SetSpeed(LEFT,0);
  MOTOR_SetSpeed(RIGHT,0);
}

void correction(float vd,float vg)
{
  if (ENCODER_Read(LEFT)<ENCODER_Read(RIGHT))
  {
    while (ENCODER_Read(RIGHT)!=ENCODER_Read(LEFT))
    {
      vg=vg+0.3;
    }
    vg=vg-0.3;
  }
  else if (ENCODER_Read(LEFT)>ENCODER_Read(RIGHT))
  {
    while (ENCODER_Read(RIGHT)!=ENCODER_Read(LEFT))
      {
        vg=vg-0.3;
      }
      vg=vg+0.3;
  }
  else
  {
    vg=vg;
  }
  
}

void deplacement(float vd, float vg, float dist, int pulse, float cirRoue)
{
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
  
  while (ENCODER_Read(RIGHT)!=pulse)  //tant que les pulses du moteur != la distance il avance et corrige la vitesse moteur gauche
  {
    avance(vd,vg);
    correction(vd,vg);
  }
  stop(vd, vg);
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}
