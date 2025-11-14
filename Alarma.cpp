#include "Alarma.h"

Alarma::Alarma(int buz, int led, int btn)
    : buzzerPin(buz), ledPin(led), botonPin(btn), estado(0) {
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(botonPin, INPUT_PULLUP);  // Evita ruido, evita falsas lecturas
}

void Alarma::activar() {
  if (estado == 0) {
    estado = 1;
    tone(buzzerPin, 440);
    digitalWrite(ledPin, HIGH);
    Serial.println("Movimiento detectado → Alarma ON");
  }
}

void Alarma::apagar() {
  if (estado == 1) {
    delay(200);
    estado = 0;
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
    Serial.println("Alarma apagada manualmente");
  }
}

void Alarma::chequearBoton() {
  if (!digitalRead(botonPin)) { // Botón presionado
      apagar();
  }
}

int Alarma::getEstado() const {
  return estado;
}
