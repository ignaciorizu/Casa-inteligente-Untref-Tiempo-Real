#include "SensorTemperatura.h"

SensorTemperatura::SensorTemperatura(int pinDHT, int pinLED, float tMin, float tMax)
  : dht(pinDHT, DHT22), ledPin(pinLED), tempMin(tMin), tempMax(tMax), ultimaTemp(NAN) {
    dht.begin();
    pinMode(ledPin, OUTPUT);
  }

void SensorTemperatura::actualizar() {
  float t = dht.readTemperature();
  if (!isnan(t)) {
    ultimaTemp = t;
    Serial.printf("Temp (pin %d): %.2f °C\n", ledPin, t);
  }
}

float SensorTemperatura::getTemperatura() const {
  return ultimaTemp;
}
