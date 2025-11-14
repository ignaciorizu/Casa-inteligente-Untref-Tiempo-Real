#include "SensorMovimiento.h"

SensorMovimiento::SensorMovimiento(int pinPIR)
    : pin(pinPIR) {
  pinMode(pin, INPUT_PULLUP);   // Funciona mejor en Wokwi con PIR open-collector
}

bool SensorMovimiento::hayMovimiento() {
  return digitalRead(pin);   // HIGH = movimiento detectado
}
