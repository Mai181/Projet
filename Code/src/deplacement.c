#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>


int main()
{
  float vd=0.7;
  float vg=0.7;
  float avance(float vd,float vg);
  float deplacement(float vd,float vg);
  return 0;
}
float avance(float vd, float vg)
{
  MOTOR_SetSpeed(0,vg);
  MOTOR_SetSpeed(1,vd);
  return 0;
}
float deplacement(float vd,float vg)
{
  for (int i=0; i<10; i++)
  {
    if (ENCODER_Read(0)<ENCODER_Read(1))
    {
      while (ENCODER_Read(1)!=ENCODER_Read(0))
      {
       vg=vg+0.3;
     }
    }
    else if (ENCODER_Read(0)>ENCODER_Read(1))
    {
      while (ENCODER_Read(1)!=ENCODER_Read(0))
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
  return 0;

}
