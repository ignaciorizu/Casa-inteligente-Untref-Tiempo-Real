#include "Alarma.h"

Alarma::Alarma(int speaker, int led)
    : pinSpeaker(speaker), pinLed(led), activa(false)
{
  pinMode(pinSpeaker, OUTPUT);
  pinMode(pinLed, OUTPUT);
}

void Alarma::activar(String zona) {
  if (!activa) {
    activa = true;
    Serial.print("ALARMA ACTIVADA por detección en: ");
    Serial.println(zona);
  }
}

void Alarma::desactivar() {
  if (activa) {
    activa = false;
    noTone(pinSpeaker);
    digitalWrite(pinLed, LOW);
    Serial.println("Botón presionado → Alarma DESACTIVADA");
  }
}

void Alarma::actualizar() {
  if (activa) {
    tone(pinSpeaker, 440);
    digitalWrite(pinLed, HIGH);
  }
}

bool Alarma::estaActiva() const {
  return activa;
}
