/********
    Fichier: avancement.h
    Auteurs: Florent Blanchard
    Date: 26 septembre 2024
    Description: déclaration des fonctions d'avancement
********/

// Fonction pour déclarer les vitesses
void avance(float vd, float vg);
float pulseDistance(float dist,int ptr, float cirRoue);
// Fonction pour ajuster les roues
void deplacement(float vd, float vg, float dist, int pulse, float cirRoue);
void correction(float vd,float vg);
void stop(float vd, float vg);
void accel(float vd, float vg);
