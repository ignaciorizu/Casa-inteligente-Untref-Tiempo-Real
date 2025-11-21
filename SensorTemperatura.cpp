#include "SensorTemperatura.h"

SensorTemperatura::SensorTemperatura(int pinDHT, int pinLED, ConfigManager& cfg)
  : dht(pinDHT, DHT22), ledPin(pinLED), config(cfg), ultimaTemp(NAN)
{
  dht.begin();
  pinMode(ledPin, OUTPUT);
}

void SensorTemperatura::actualizar() {
  float t = dht.readTemperature();
  if (!isnan(t)) {
    ultimaTemp = t;
  }
  actualizarLED();
}

void SensorTemperatura::actualizarLED() {
  if (isnan(ultimaTemp)) return;

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
