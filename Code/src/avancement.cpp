#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <avancement.h>

void avance(float vd, float vg)
{
  MOTOR_SetSpeed(LEFT,vg);
  MOTOR_SetSpeed(RIGHT,vd);
}
void deplacement(float vd,float vg)
{
  for (int i=0; i<10; i++)
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


}