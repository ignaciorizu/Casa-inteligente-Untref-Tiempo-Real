#include <Arduino.h>
#include "../include/CasaInteligente.h"

CasaInteligente casa;

void setup() {
    Serial.begin(115200);

    casa.iniciar();
    casa.crearTareas();
}

void loop() {}
