#include <Arduino.h>
#include <librobus.h>
#include <stdio.h>
#include <detectionSifflet.h>

bool detectionSifflet(){

    bool sifflet = false;

    int bruitAmbiant = analogRead(A10); //l'information lue à la pin 10 se trouve entre 0 et 1023
    float voltageBruitAmbiant = bruitAmbiant * (5.0 / 1023.0); //produit croisé pour ramener sur 5V (l'alim du circuit)

    int cinqkH = analogRead(A11);
    float voltage5kH = cinqkH * (5.0 / 1023.0);

    if (voltage5kH > voltageBruitAmbiant){
        sifflet = true;
    }

    return sifflet;
}