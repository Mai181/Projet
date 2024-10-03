#include <Arduino.h>
#include <librobus.h>
#include <Rotations.h>

void setup() {
	BoardInit();
}

void loop() {
  rotationDroite(90);
  delay(5);
  rotationGauche(90);
  delay(5);
  exit;
}
