#include "Alarma.h"

Alarma::Alarma(int speaker, int led)
    : pinSpeaker(speaker), pinLed(led), activa(false)
{
  pinMode(pinSpeaker, OUTPUT);
  pinMode(pinLed, OUTPUT);
}

void Alarma::activar() {
  if (!activa) {
    activa = true;
    Serial.println("ALERTA: Movimiento detectado → Alarma ACTIVADA");
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
