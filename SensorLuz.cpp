#include "SensorLuz.h"

SensorLuz::SensorLuz(int pinLDR_, int pinLED_, const char* nombreZona, ConfigManager& cfg)
  : ldrPin(pinLDR_), ledPin(pinLED_), nombre(nombreZona), luxValue(NAN), alpha(0.2f), config(cfg) 
{
  pinMode(ledPin, OUTPUT);
}

// Convierte ADC (0..4095) a voltaje (0..VCC)
float SensorLuz::adcToVoltage(int adc) const {
  // Aseguramos rango 0..4095
  if (adc < 0) adc = 0;
  if (adc > 4095) adc = 4095;
  return ( (float)adc / 4095.0f ) * VCC;
}

// Calcula resistencia del LDR a partir del voltaje en divisor: Vout = VCC * Rldr / (R + Rldr)
// => Rldr = R * Vout / (VCC - Vout)
float SensorLuz::voltageToResistance(float voltage) const {
  if (voltage <= 0.0f) return 1e9f; // muy alta resistencia (oscuridad extrema)
  if (voltage >= VCC) return 1e-3f; // muy baja resistencia (mucha luz)
  return (R_FIXED * voltage) / (VCC - voltage);
}

// Convierte resistencia a lux usando la misma fórmula que tu Python:
// lux = (RL10*1000 * 10^GAMMA / Rldr)^(1/GAMMA)
float SensorLuz::resistanceToLux(float resistance) const {
  // RL10 está en kΩ en tu código Python; RL10*1000 lo deja en ohm
  float numerator = (RL10 * 1000.0f) * powf(10.0f, GAMMA);
  float ratio = numerator / resistance;
  if (ratio <= 0.0f) return 0.0f;
  float lux = powf(ratio, 1.0f / GAMMA);
  return lux;
}

void SensorLuz::actualizar() {
  int adc = analogRead(ldrPin);         // 0..4095
  float v = adcToVoltage(adc);          // V
  float r_ldr = voltageToResistance(v); // ohmios
  float lux = resistanceToLux(r_ldr) / 10.0f;   // lux

  // Suavizado exponencial (EMA) para evitar ruido; alpha en [0..1]
  if (isnan(luxValue)) {
    luxValue = lux;
  } else {
    luxValue = (alpha * lux) + (1.0f - alpha) * luxValue;
  }

  if (luxValue < config.getUmbralLux()) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}

float SensorLuz::getLux() const {
  return luxValue;
}
