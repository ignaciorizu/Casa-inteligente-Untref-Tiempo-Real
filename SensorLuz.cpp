#include "SensorLuz.h"
#include <Arduino.h>

SensorLuz::SensorLuz(int ldr, int led) {
  ldrPin = ldr;
  ledPin = led;
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void SensorLuz::actualizar() {
  int light = digitalRead(ldrPin);
  digitalWrite(ledPin, light);
}