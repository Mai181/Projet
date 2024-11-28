/*    
*   Fichier: main.cpp
*   Auteurs: P-19
*   Date: 11 novembre 2024
*   Description: code PRINCIPAL du robot
*/

#include "header.h"

void setup(){

    BoardInit();
    DisplayInit();
    Serial.begin(9600);
    Wire.begin();
    pinMode(PINROUGE, OUTPUT);  
    pinMode(PINVERT, OUTPUT);  
    pinMode(PINORANGE, OUTPUT);    
    pinMode(PIN_BOUTON_1_CHANGE_GAUCHE, INPUT);  
    pinMode(PIN_BOUTON_2_CHANGE_DROITE, INPUT);  
    pinMode(PIN_BOUTON_3_SELECT, INPUT);  
    pinMode(PIN_BOUTON_4_RETOUR, INPUT);  
    pinMode(PIN_SERVO_DISTRIBUTEUR, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_1, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_2, INPUT);  
    pinMode(PIN_ANALOG_DETECTEUR_METAUX_3, INPUT);  
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
    INIT_servos();
    allumerDEL(RIEN, true);
    affichageLCD(true, "P-19 IronMinds  ////                 ");
    delay(DELAIS*50);
    affichageLCD(true, "Menu principal  ////>Commencer  Regl ");

    /*
avancer(1);

int dimensionXTemp = 50;
bool enTransitionTemp = true;
float accelerationTempsActionTemp = 300;
float accelerationTempsTemp = accelerationTempsActionTemp;
int accelerationPhaseTemp = 1;

float distanceAReduireTemp = roueDistance + 5;

avancer(vitesseIntermediaireDiviseur1);
    while((-(ENCODER_Read(LEFT)+ENCODER_Read(RIGHT))/2)*roueCirconference/rouePulseCirconference<dimensionXTemp - distanceAReduireTemp)
    {
                if(enTransitionTemp)
                {
                    if(accelerationTempsTemp < 0 )
                    {
                        accelerationTempsTemp += accelerationTempsActionTemp;
                        accelerationPhaseTemp++;

                        if(accelerationPhaseTemp==5)
                            enTransitionTemp = false;
                        
                    }
                    else
                        accelerationTempsTemp -= DELAIS;
                }

                switch(accelerationPhaseTemp)
                {
                    case 0 || 1:
                        ajustementVitesse(vitesseIntermediaireDiviseur1);
                        break;
                    case 2:
                        ajustementVitesse(vitesseIntermediaireDiviseur2);
                        break;
                    case 3:
                        ajustementVitesse(vitesseIntermediaireDiviseur3);
                        break;
                    case 4:
                        ajustementVitesse(vitesseIntermediaireDiviseur4);
                        break;
                    case 5:
                        ajustementVitesse(1);
                        break;
                }
                delay(DELAIS);
    }
delay(DELAIS*10);
rotation(GAUCHE);
resetEncodeur();
delay(DELAIS*10);
rotation(GAUCHE);
    */
}

void loop(){
/*
        if(detectionMetaux()){
            distributeur(true);
            allumerDEL(METAL, true);
        }
        else
        {
            distributeur(false);
            allumerDEL(METAL, false);
        }
*/
/*
rotation(DROITE);
delay(DELAIS*10);
rotation(DROITE);
delay(DELAIS*10);
ajustementVitesse(1);
delay(DELAIS);
*/
//^^^^^^^^^^test^^^^^^^^

/*
*/
    boutonsUpdate();
    arbreDecision();
    delay(DELAIS);
}
