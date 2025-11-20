#ifndef SENSOR_LUZ_H
#define SENSOR_LUZ_H

#include <Arduino.h>
#include "ConfigManager.h"

class SensorLuz {
public:
    SensorLuz(int pinLDR_, int pinLED_, const char* nombreZona, ConfigManager& cfg);

    void actualizar();
    float getLux() const;

private:
  int ldrPin;
  int ledPin;
  const char* nombre;
  ConfigManager& config;

  // Parámetros del modelo (tomados del ejemplo Python)
  static constexpr float VCC = 3.3f;     // Voltaje de referencia
  static constexpr float R_FIXED = 2000.0f;  // R en ohmios (p.ej. 2000)
  static constexpr float RL10 = 50.0f;   // kΩ a 10 lux -> usar 50 (kΩ) como en Python
  static constexpr float GAMMA = 0.7f;

  float luxValue;
  float alpha;
  
  // Helpers
  float adcToVoltage(int adc) const;
  float voltageToResistance(float voltage) const;
  float resistanceToLux(float resistance) const;
};

#endif
