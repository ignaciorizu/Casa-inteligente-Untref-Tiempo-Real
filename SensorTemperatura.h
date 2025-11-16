#ifndef SENSOR_TEMPERATURA_H
#define SENSOR_TEMPERATURA_H

#include <Arduino.h>
#include <DHT.h>

class SensorTemperatura {
  private:
    DHT dht;
    int ledPin;
    float tempMin;
    float tempMax;
    float ultimaTemp;

  public:
    SensorTemperatura(int pinDHT, int pinLED, float tMin = 20.0, float tMax = 24.0);
    void actualizar();
    float getTemperatura() const;
    int getLedPin() const { return ledPin; }
};


#endif
