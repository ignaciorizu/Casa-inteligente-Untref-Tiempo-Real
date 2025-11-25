#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#ifdef UNIT_TEST
    #include "./test/mocks/ArduinoMock.h"
    #include "./test/mocks/HardwareMock.h"
#else
    #include <Arduino.h>
    #include <DHT.h>
#endif

#include "ConfigManager.h"

class SensorTemperatura {
  public:
    SensorTemperatura(int pinDHT, int pinLED, ConfigManager& cfg);
    void actualizar();
    void actualizarLED();
    float getTemperatura() const;
    int getLedPin() const { return ledPin; }

    #ifdef UNIT_TEST
    const DHT& getDHTForTesting() const { return dht; }
    #endif

  private:
    ConfigManager& config;
    DHT dht;
    int ledPin;
    float ultimaTemp;
    int ultimoEstadoLED = LOW;
};


#endif
