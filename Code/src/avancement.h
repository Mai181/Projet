/********
    Fichier: avancement.h
    Auteurs: Florent Blanchard
    Date: 26 septembre 2024
    Description: déclaration des fonctions d'avancement
********/

// Fonction pour déclarer les vitesses
void decel(int pulse, float vd, float vg);
float avance(float dist, float distanceParcourue);
void CorrigerVitesse(float vd,float vg, float distanceParcourue);
void stop();
void accel(float vd, float vg);
