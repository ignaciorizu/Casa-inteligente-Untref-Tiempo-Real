#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include "../Interfaces/ISensorTemperatura.h"

#ifdef UNIT_TEST
    // En tests, no incluir DHT
#else
    #include <Arduino.h>
    #include <DHT.h>
#endif

class SensorTemperatura : public ISensorTemperatura {
  private:
    #ifndef UNIT_TEST
    DHT dht;
    #endif
    int ledPin;
    float tempMin;
    float tempMax;
    float ultimaTemp;

  public:
    SensorTemperatura(int pinDHT, int pinLED, float tMin = 20.0, float tMax = 24.0);
    void actualizar() override;
    float getTemperatura() const override;
    int getLedPin() const { return ledPin; }
};
#endif