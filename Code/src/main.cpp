/********
    Fichier: main.cpp
    Auteurs: P-19
    Date: 26 septembre 2024
    Description: code principal des robots
********/

#include <Arduino.h>
#include <librobus.h>
#include <Rotations.h>
#include <detectionSifflet.h>
#include <math.h>
#include <detectionObstacleDevant.h>
#include <detectionObstacleDroite.h>

const int dt=500;//différence de temps
bool obstacleAvant, obstacleDroit, obstacleGauche; //Indique s'il y a un obstacle dans la direction
bool ligneDepart, ligneFin; //Indique que le robot est a la ligne départ ou de fin
bool ruban; //Indique qu'il y a un ruban a cette rangé
int direction = 0; //orientation du robot dans l'espace
int range = 25; //postion en Y (au départ à 25cm)

int colonne =75; //positon en X (au départ à 75cm)
bool depart = 0; //signal du siflet detecté
bool siffletActive = false;

// Paramètres du régulateur PI
float erreurAccumuleeGauche = 0;  // Somme des erreurs accumulées pour la roue gauche
float erreurAccumuleeDroite = 0;  // Somme des erreurs accumulées pour la roue droite
float totalpulseDroit;
float totalpulseGauche;
float Ki1 = 0.005;  // Gain intégral
float Kp1 = 0.0015;  // Gain proportionnel pour A
float deltaT = 0.05;  // Intervalle de temps entre les cycles (en secondes)


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
    for (int i = 1; i <= 5; i++) {
        CorrigerVitesse(i * (vd / 5), i * (vg / 5));
        delay(150);  // Attendre un peu entre chaque étape d'accélération
    }
}

// Fonction de décélération progressive
void decel(int pulse, float vd, float vg) {
    int pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

    while (pulsesRestants > 100) {
        // Calcul des pulses restants
        pulsesRestants = pulse - ((ENCODER_Read(RIGHT) + ENCODER_Read(LEFT)) / 2);

        // Réduire la vitesse proportionnellement à la distance restante
        float facteurDecel = ((float)pulsesRestants * 5 ) / pulse;
        if (facteurDecel < 0.15) {
            facteurDecel = 0.15;  // Ne pas descendre en dessous de 10% de la vitesse
        }

        // Appliquer la réduction de la vitesse
        float vitesseDroit = vd * facteurDecel;
        float vitesseGauche = vg * facteurDecel;

        MOTOR_SetSpeed(RIGHT, vitesseDroit);
        MOTOR_SetSpeed(LEFT, vitesseGauche);
/*        Serial.print("vitesse droit decel:");
        Serial.println(vitesseDroit);
        Serial.print("vitesse gauche decel:");
        Serial.println(vitesseGauche);
*/
        delay(50);  // Attendre entre chaque ajustement de la vitesse
    }

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
    int pulseArret = 10 * pulseParCM;  // Distance nécessaire pour arrêter (en pulses)



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
    Serial.print("Total Pulses Droit: ");
    Serial.println(totalpulseDroit);
    Serial.print("Total Pulses Gauche: ");
    Serial.println(totalpulseGauche);
    Serial.print("Nombre de pulses calculé : ");
    Serial.println(pulse);
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

  //Calcul de la position des rubans
  if((range % 100) >= 40){
    ruban = 1;
  }
  else {
    ruban = 0;
  }

  //Calcul du depart du parcours
  if(range == 25){
    ligneDepart = 1;
  }
  else{
    ligneDepart = 0;
  }

  //Calcul de la Fin du parcours
  if(range == 525){
    rotationDroite(180);
    ligneFin = 1;
  }
//si arriver a la fin
  if(ligneFin == 1 && range == 25){
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
  if(capteurDroit() == 1 || (colonne == 125 && direction == 0) || (ligneDepart == 1 && direction == 90) || ruban == 1){
    obstacleDroit = 1;
    Serial.println("obstacle droit");
  }
  else {
    obstacleDroit = 0;
  }

  //Calcul des obstacle à la gauche du robot
  if((colonne == 25 && direction == 0) || (ligneDepart == 1 && direction == 270) || ruban == 1){
    obstacleGauche = 1;
    Serial.println("obstacle gauche");
  }
  else {
    obstacleGauche = 0;
  }

  //Calcul des obstacle à l'avant du robot
  if(capteurAvant() == 1 || (ligneDepart == 1 && direction == 180) || (colonne == 125 && direction == 90) 
    || (colonne == 25 && direction == 270) || (ruban == 1 &&(direction == 90 || direction == 270))){
    obstacleAvant = 1;
    Serial.println("obstacle avant");
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