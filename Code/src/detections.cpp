/********
    Fichier: main.cpp
    Auteurs: P-19
    Date: 26 septembre 2024
    Description: code des détections des capteurs et des sifflets
********/

#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <detections.h>

bool capteurAvant(){

    bool obstacleAvant = false;
    uint16_t lectureGaucheA;
    uint16_t lectureDroiteA;

    lectureGaucheA = ROBUS_ReadIR(0);
    lectureDroiteA = ROBUS_ReadIR(1);

    if (lectureGaucheA < 500 && lectureDroiteA < 500){
        obstacleAvant = true;
    }

    return obstacleAvant;
}

bool capteurDroit(){

    bool obstacleDroite = false;
    uint16_t lectureGaucheD;
    uint16_t lectureDroiteD;

    lectureGaucheD = ROBUS_ReadIR(2);
    lectureDroiteD = ROBUS_ReadIR(3);

    if (lectureGaucheD < 500 && lectureDroiteD < 500){
        obstacleDroite = true;
    }

    return obstacleDroite;
}

bool detectionSifflet(){

    bool sifflet = false; //par défaut, il n'y a pas de sifflet

    int bruitAmbiant = analogRead(A10); //l'information lue à la pin 10 se trouve entre 0 et 1023
    float voltageBruitAmbiant = bruitAmbiant * (5.0 / 1023.0); //produit croisé pour ramener sur 5V (l'alim du circuit)
//    Serial.println(voltageBruitAmbiant);

    int cinqkH = analogRead(A11);
    float voltage5kH = cinqkH * (5.0 / 1023.0);
//    Serial.println(voltage5kH);

    if (voltage5kH > voltageBruitAmbiant){
        sifflet = true;
    }

    return sifflet;
}
