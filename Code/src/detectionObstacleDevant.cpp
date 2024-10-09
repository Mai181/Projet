#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <detectionObstacleDevant.h>

bool capteurAvant(){

    bool obstacleDevant = false;
    uint16_t lectureGauche;
    uint16_t lectureDroite;

    lectureGauche = ROBUS_ReadIR(0);
    lectureDroite = ROBUS_ReadIR(1);

    if (lectureGauche < 500 && lectureDroite < 500){
        obstacleDevant = true;
    }

    return obstacleDevant;
}