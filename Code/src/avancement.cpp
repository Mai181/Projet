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
#include <rotation.cpp>
float dist=50.0;  //la distance entre deux centre de carre

float pulseDistance(float dist,int pulseTourRoue, float cirRoue) //dis= distance a parcourire en cm (du centre d'un carre a un autre est de 50cm)
{
  int pulse=(pulseTourRoue/cirRoue)*dist;
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
  }
  else if (ENCODER_Read(LEFT)>ENCODER_Read(RIGHT))
  {
    while (ENCODER_Read(RIGHT)!=ENCODER_Read(LEFT))
      {
        vg=vg-0.3;
      }
  }
  else
  {
    vg=vg;
  }
  delay(1000);
  
}

void deplacement(float vd, float vg, float dist, int pulse, float cirRoue)
{
  pulse=pulseDistance(dist, pulseTourRoue, cirRoue);
  while (ENCODER_Read(RIGHT)!=pulseDistance(dist,pulseTourRoue,cirRoue))  //tant que les pulses du moteur != la distance il avance et corrige la vitesse moteur gauche
  {
   while (ENCODER_Read(RIGHT)!= 10)
   {
    avance(vd,vg);
    correction(vd,vg);
   }
  }
  stop(vd, vg);
  ENCODER_Reset(RIGHT);
  ENCODER_Reset(LEFT);
}
