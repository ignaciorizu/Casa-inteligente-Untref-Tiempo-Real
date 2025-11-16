#ifndef SENSOR_LUZ_H
#define SENSOR_LUZ_H

#include <Arduino.h>

class SensorLuz {
public:
  // Umbral en lux (puedes cambiarlo)
  static constexpr float UMBRAL_LUX = 100.0f;

  // Constructor: pinLDR = ADC pin (p. ej. 32), pinLED = salida digital, nombre para debug
  SensorLuz(int pinLDR, int pinLED, const char* nombreZona);

  // Lee el ADC, calcula lux, controla el LED y actualiza el valor interno
  void actualizar();

  // Devuelve el lux calculado (valor filtrado si se usa smoothing)
  float getLux() const;

private:
  int ldrPin;
  int ledPin;
  const char* nombre;

  // Parámetros del modelo (tomados del ejemplo Python)
  static constexpr float VCC = 3.3f;     // Voltaje de referencia
  static constexpr float R_FIXED = 2000.0f;  // R en ohmios (p.ej. 2000)
  static constexpr float RL10 = 50.0f;   // kΩ a 10 lux -> usar 50 (kΩ) como en Python
  static constexpr float GAMMA = 0.7f;

  float luxValue;      // valor actual de lux
  float alpha;         // factor EMA (smoothing), 0=no smoothing, 1=solo nueva lectura

  // Helpers
  float adcToVoltage(int adc) const;
  float voltageToResistance(float voltage) const;
  float resistanceToLux(float resistance) const;
};

#endif
