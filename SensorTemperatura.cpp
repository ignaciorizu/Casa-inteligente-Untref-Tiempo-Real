#include "SensorTemperatura.h"

#ifdef UNIT_TEST
    #include <cmath> // Para std::isnan
#endif

SensorTemperatura::SensorTemperatura(int pinDHT, int pinLED, ConfigManager& cfg)
  : config(cfg),
    dht(pinDHT, DHT22),
    ledPin(pinLED),
    ultimaTemp(NAN)
{
  #ifndef UNIT_TEST
  dht.begin();
  #endif
  pinMode(ledPin, OUTPUT);
}

void SensorTemperatura::actualizar() {
  float t = dht.readTemperature();
  #ifdef UNIT_TEST
  if (!std::isnan(t)) {
  #else
  if (!isnan(t)) {
  #endif
    ultimaTemp = t;
  }
  actualizarLED();
}

void SensorTemperatura::actualizarLED() {
  #ifdef UNIT_TEST
  if (std::isnan(ultimaTemp)) return;
  #else
  if (isnan(ultimaTemp)) return;
  #endif

  float tMin = config.getTempMin();
  float tMax = config.getTempMax();

  if (ultimaTemp < tMin) {
    ultimoEstadoLED = HIGH;
  } else if (ultimaTemp > tMax) {
    ultimoEstadoLED = LOW;
  }

  digitalWrite(ledPin, ultimoEstadoLED);
}

float SensorTemperatura::getTemperatura() const {
  return ultimaTemp;
}
