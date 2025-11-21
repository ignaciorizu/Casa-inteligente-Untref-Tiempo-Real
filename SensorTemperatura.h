#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include <Arduino.h>
#include <DHT.h>
#include "ConfigManager.h"

class SensorTemperatura {
  public:
    SensorTemperatura(int pinDHT, int pinLED, ConfigManager& cfg);
    void actualizar();
    void actualizarLED();
    float getTemperatura() const;
    int getLedPin() const { return ledPin; }

  private:
    ConfigManager& config;
    DHT dht;
    int ledPin;
    float ultimaTemp;
    int ultimoEstadoLED = LOW;
};


#endif
