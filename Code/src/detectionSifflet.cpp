#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <detectionSifflet.h>

bool detectionSifflet(){

    bool sifflet = false; //par défaut, il n'y a pas de sifflet

    int bruitAmbiant = analogRead(A10); //l'information lue à la pin 10 se trouve entre 0 et 1023
    float voltageBruitAmbiant = bruitAmbiant * (5.0 / 1023.0); //produit croisé pour ramener sur 5V (l'alim du circuit)
    Serial.println(voltageBruitAmbiant);

    int cinqkH = analogRead(A11);
    float voltage5kH = cinqkH * (5.0 / 1023.0);
    Serial.println(voltage5kH);

    if (voltage5kH > voltageBruitAmbiant){
        sifflet = true;
    }

    return sifflet;
}