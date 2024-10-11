/********
    Fichier: main.cpp
    Auteurs: P-19
    Date: 26 septembre 2024
    Description: code principal des robots
********/

#include <Arduino.h>
#include <librobus.h>
#include <Rotations.h>
#include <math.h>
#include <detections.h>

const int dt=500;//différence de temps
bool obstacleAvant, obstacleDroit, obstacleGauche; //Indique s'il y a un obstacle dans la direction
bool ligneDepart, ligneFin; //Indique que le robot est a la ligne départ ou de fin
bool ruban; //Indique qu'il y a un ruban a cette rangé
int direction = 0; //orientation du robot dans l'espace
int range = 25; //postion en Y (au départ à 25cm)

int colonne = 75; //positon en X (au départ à 75cm)
bool depart = false; //signal du siflet detecté
bool siffletActive = false;

// Paramètres du régulateur PI
float erreurAccumuleeGauche = 0;  // Somme des erreurs accumulées pour la roue gauche
float erreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite
float totalpulseDroit;
float totalpulseGauche;
float Ki1 = 0.0005;  // Gain intégral pour A ne pas oublier d'enlever la valeur -0.0005 a la ligne 149 de la vitesse de decel
float Kp1 = 0.0012;  // Gain proportionnel pour A
//float Ki1 = 0.0006;  // Gain intégral pour B elever 200 pulse pour la fin
//float Kp1 = 0.0017;  // Gain proportionnel pour B
float deltaT = 0.05;  // Intervalle de temps entre les cycles (en secondes)

// Matrice pour les obstacles du parcours
int rubans[23][7] = {{1, 1, 1, 1, 1, 1, 1},  // x=0   (0)
                    {1, 0, 0, 0, 0, 0, 1},   // x=25  (1)
                    {1, 0, 1, 1, 1, 0, 1},   // x=50  (2)
                    {1, 0, 1, 0, 1, 0, 1},   // x=75  (3)
                    {1, 0, 1, 1, 1, 0, 1},   // x=100 (4)
                    {1, 0, 0, 0, 0, 0, 1},   // x=125 (5)
                    {1, 0, 1, 1, 1, 0, 1},   // x=150 (6)
                    {1, 0, 1, 0, 1, 0, 1},   // x=175 (7)
                    {1, 0, 1, 1, 1, 0, 1},   // x=200 (8)
                    {1, 0, 0, 0, 0, 0, 1},   // x=225 (9)
                    {1, 0, 1, 0, 1, 0, 1},   // x=250 (10)
                    {1, 0, 1, 0, 1, 0, 1},   // x=275 (11)
                    {1, 0, 1, 0, 1, 0, 1},   // x=300 (12)
                    {1, 0, 0, 0, 0, 0, 1},   // x=325 (13)
                    {1, 0, 1, 1, 1, 0, 1},   // x=350 (14)
                    {1, 0, 1, 0, 1, 0, 1},   // x=375 (15)
                    {1, 0, 1, 1, 1, 0, 1},   // x=400 (16)
                    {1, 0, 0, 0, 0, 0, 1},   // x=425 (17)
                    {1, 0, 1, 1, 1, 0, 1},   // x=450 (18)
                    {1, 0, 1, 0, 1, 0, 1},   // x=475 (19)
                    {1, 0, 1, 0, 1, 0, 1},   // x=500 (20)
                    {1, 0, 0, 0, 0, 0, 1},   // x=525 (21)
                    {1, 1, 1, 1, 1, 1, 1}};  // x=550 (22)  ***index range = x/25
            // index 0, 1, 2, 3, 4, 5, 6 
            // y =  0,25,50,75,100,125,150  ***index colonne = y/25

int verifCol[4][3] = {{25, -25, 0},  // direction = 0
                      {0, 0, 25},     // direction = 90
                      {-25, 25, 0},   // direction = 180
                      {0, 0, -25}};    // direction = 270
                // Droit, Gauche, Avant

int verifRan[4][3] = {{0, 0, 25},   // direction = 0
                      {-25, 25, 0},   // direction = 90
                      {0, 0, -25},   // direction = 180
                      {25, -25, 0}};  // direction = 270
                // Droit, Gauche, Avant

// Fonction pour limiter une valeur dans une plage donnée
float limiter(float valeur, float minVal, float maxVal) {
    if (valeur > maxVal) return maxVal;
    if (valeur < minVal) return minVal;
    return valeur;
}

// Lecture de la vitesse de la roue droite
float lireVitesseDroit() {
    int32_t posInit = ENCODER_Read(RIGHT);
    delay(deltaT * 1000);
    float vitesse = (ENCODER_Read(RIGHT) - posInit) / 50.0;
    return vitesse;
}

// Lecture de la vitesse de la roue gauche
float lireVitesseGauche() {
    int32_t posInit = ENCODER_Read(LEFT);
    delay(deltaT * 1000);
    float vitesse = (ENCODER_Read(LEFT) - posInit) / 50.0;
    return vitesse;
}

// Fonction de régulation PI pour ajuster les vitesses des moteurs
void CorrigerVitesse(float vd, float vg) {
    totalpulseDroit = ENCODER_Read(RIGHT);
    totalpulseGauche = ENCODER_Read(LEFT);

    // Calcul des écarts entre la consigne et la mesure
    float ecartDroit = totalpulseGauche - totalpulseDroit;
    Serial.print("ecrat:");
    Serial.println(ecartDroit);

    // Calcul des termes proportionnels
    float termePropDroit = ecartDroit * Kp1;
    Serial.print("termePropDroit:");
    Serial.println(termePropDroit);
    // Mise à jour des erreurs accumulées pour l'intégrale
    erreurAccumuleeDroite += ecartDroit * deltaT;
    Serial.print("erreur acc:");
    Serial.println(erreurAccumuleeDroite);
    //calcul de erreur position total
    

    // Calcul des termes intégrals
    float termeIntDroit = erreurAccumuleeDroite * Ki1;
    Serial.print("termeintDroit:");
    Serial.println(termeIntDroit);
    // Calcul des corrections finales en limitant la vitesse pour éviter des valeurs trop élevées
    float correctionDroit = limiter(vd + termePropDroit + termeIntDroit, -1.0, 1.0);
/*    Serial.print("vitesse droit:");
    Serial.println(correctionDroit);
    Serial.print("vitesse gauche:");
    Serial.println(correctionGauche);
*/
    // Application des corrections aux moteurs
    MOTOR_SetSpeed(RIGHT, correctionDroit);
    MOTOR_SetSpeed(LEFT, vg);
    Serial.println(correctionDroit);
}
// Fonction pour l'accélération progressive
void accel(float vd, float vg) {
    for (int i = 1; i <= 10; i++) {
        CorrigerVitesse(i * (vd / 10), i * (vg / 10));
        delay(75);  // Attendre un peu entre chaque étape d'accélération
    }
}



// Fonction de décélération progressive
void decel(int pulse, float vd, float vg) {
    int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
    int i = 4;

    while (pulsesRestants > 200) {
          if (i >= 1){
          CorrigerVitesse(i * (vd / 5), i * (vg / 5));
          delay(50);  // Attendre un peu entre chaque étape d'accélération
          i--;
          }
          pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);
      }
/*        Serial.print("vitesse droit decel:");
        Serial.println(vitesseDroit);
        Serial.print("vitesse gauche decel:");
        Serial.println(vitesseGauche);
*/

    // Assurer l'arrêt complet des moteurs
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

// Fonction pour arrêter les moteurs
void stop() {
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

// Fonction pour faire avancer le robot sur une distance donnée
// Fonction pour faire avancer le robot sur une distance donnée
void avance(float dist) {
    float vd = 0.6;  // Vitesse désirée droite
    float vg = 0.6;  // Vitesse désirée gauche
    int ptr = 3200;  // Nombre de pulses par rotation
    float circRoue = 23.94;  // Circonférence de la roue en cm
    float pulseParCM = ptr / circRoue;  // Calcul du nombre de pulses par cm
    int pulse = dist * pulseParCM;  // Nombre de pulses pour la distance donnée
    int pulseArret = 15 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)

    // Réinitialisation des encodeurs
    ENCODER_Reset(RIGHT);
    ENCODER_Reset(LEFT);

    // Accélération progressive
    accel(vd, vg);
    delay(50);

    // Suivi de la distance parcourue
    float distanceParcourue = 0;

    // Avancer jusqu'à presque atteindre la distance cible
    while (((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) < (pulse - pulseArret)) {
        // Mettre à jour la distance parcourue en cm
        distanceParcourue = ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2) / pulseParCM;

        // Correction des vitesses en fonction de la distance parcourue
        CorrigerVitesse(vd, vg);

        // Pause pour laisser le temps aux corrections
        delay(50);
    }

    // Décélération progressive
    decel(pulse, vd, vg);

    // Debug : Affichage des pulses et de la position
//   Serial.print("Total Pulses Droit: ");
//    Serial.println(totalpulseDroit);
//    Serial.print("Total Pulses Gauche: ");
//    Serial.println(totalpulseGauche);
//    Serial.print("Nombre de pulses calculé : ");
//    Serial.println(pulse);
}
  
void setup() {
	BoardInit();
  Serial.begin(9600); //Communication à 9600 bits/sec
}

void loop() {

  //Ajustement de la variable de direction
  if(direction == 360){ //si la direction est de 360 degrés c'est equivalent à 0
    direction = 0;
  }

  if(direction < 0){  //si la direction est négative, on la remet sur 360
    direction = direction + 360;
  }
  if (range == 525){
    ligneFin = 1;
  }
  else{
    ligneFin = 0;
  }


//si arriver a la fin
  if(range == 525){
    if (colonne == 125){
      rotationGauche(90.0);
      delay(250);
      avance(50.0);
      delay(250);
      rotationGauche(90.0); //a modifier si pas tout a fait en ligne
    }
    else if (colonne == 25){
      rotationDroite(90.0);
      delay(250);
      avance(50.0);
      delay(250);
      rotationDroite(90.0); //a modifier si pas tout a fait en ligne      
    }
      delay(250.0);
      avance(250.0);
      delay(250.0);
      avance(250.0);
      avance(50.0);
      delay(250);
      siffletActive = false;
  }

  //Recherche du sifflet
  while(siffletActive == false){
    siffletActive = detectionSifflet();
    if (ROBUS_IsBumper(REAR) == 1){
      siffletActive = true;
    }
  }

  //Calcul des obstacle à la droite du robot
  if(capteurDroit() == 1 || ((rubans[(range + verifRan[direction/90][0])/25][(colonne + verifCol[direction/90][0])/25] == 1 && ligneFin == 0))){
    obstacleDroit = 1;
    Serial.println("obstacle droit");
  }
  else {
    obstacleDroit = 0;
  }

  //Calcul des obstacle à la gauche du robot
  if((rubans[(range + verifRan[direction/90][1])/25][(colonne + verifCol[direction/90][1])/25] == 1 && ligneFin == 0)){

    obstacleGauche = 1;
    Serial.println("obstacle gauche");
  }
  else {
    obstacleGauche = 0;
  }

  //Calcul des obstacle à l'avant du robot
  if(capteurAvant() == 1 || (rubans[(range + verifRan[direction/90][2])/25][(colonne + verifCol[direction/90][2])/25] == 1 && ligneFin == 0)){
    obstacleAvant = 1;
    Serial.println(rubans[(range + verifRan[direction/90][2])/25][(colonne + verifCol[direction/90][2])/25]);
    Serial.println((range + verifRan[direction/90][2])/25);
    Serial.println((colonne + verifCol[direction/90][2])/25);
  }
  else{
    obstacleAvant = 0;
  }

  //Déplacement vers l'avant
  if(obstacleAvant == 0){
    Serial.println("avance");
    avance(50.0);
    if(direction == 0){
      range += 50;
    }
    else if (direction == 90)
    {
      colonne += 50;
    }
    else if (direction == 180)
    {
      range -= 50;
    }
    else if (direction == 270)
    {
      colonne -= 50;
    }
  }

  //Tourner vers la droite
  if(obstacleDroit == 0 && obstacleAvant == 1){
    Serial.println("tourne droite");
    rotationDroite(90.0);
    direction += 90;
  }

  //Tourner vers la gauche
  if(obstacleGauche == 0 && obstacleDroit == 1 && obstacleAvant == 1){
    Serial.println("tourne gauche");
    rotationGauche(90.0);
    direction -= 90;
  }

  //Si il est entouré d'obstacle il tourne à droite jusqu'a trouver une sortie
  if(obstacleAvant == 1 && obstacleDroit == 1 && obstacleGauche == 1){
    Serial.println("entourer");
    rotationDroite(90.0);
    direction += 90;
  }
  delay(dt);
}