#ifndef SENSOR_LUZ_H
#define SENSOR_LUZ_H

#include <Arduino.h>

class SensorLuz {
private:
  int ldrPin;
  int ledPin;

public:
  SensorLuz(int ldr, int led);
  void actualizar();
};

#endif
