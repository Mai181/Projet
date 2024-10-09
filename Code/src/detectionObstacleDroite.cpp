#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <detectionObstacleDroite.h>

bool capteurDroit(){

    bool obstacleDroite = false;
    uint16_t lectureGauche;
    uint16_t lectureDroite;

    lectureGauche = ROBUS_ReadIR(2);
    lectureDroite = ROBUS_ReadIR(3);

    if (lectureGauche < 500 && lectureDroite < 500){
        obstacleDroite = true;
    }

    return obstacleDroite;
}