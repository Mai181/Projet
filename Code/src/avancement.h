/********
    Fichier: avancement.h
    Auteurs: Florent Blanchard
    Date: 26 septembre 2024
    Description: déclaration des fonctions d'avancement
********/

// Fonction pour déclarer les vitesses
void decel(float vd, float vg);
void avance(float dist);
void CorrigerVitesse(float vd,float vg);
void stop();
void accel(float vd, float vg);
