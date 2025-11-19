#include "SensorTemperatura.h"

#ifdef UNIT_TEST
    #include <iostream>
    #include <random>
    #include "../../test/test_common_core.h"  // ✅ Solo incluir
#else
    #include <Arduino.h>
    #include <DHT.h>
#endif

SensorTemperatura::SensorTemperatura(int pinDHT, int pinLED, float tMin, float tMax)
  :
    #ifndef UNIT_TEST
    dht(pinDHT, DHT22),
    #endif
    ledPin(pinLED), tempMin(tMin), tempMax(tMax), ultimaTemp(NAN)
{
  #ifndef UNIT_TEST
  dht.begin();
  pinMode(ledPin, OUTPUT);
  #endif

  #ifdef UNIT_TEST
  std::cout << "🔧 SensorTemperatura creado (TEST) - Pin DHT: " << pinDHT << ", Pin LED: " << pinLED << std::endl;
  #else
  Serial.print("🔧 SensorTemperatura creado (REAL) - Pin DHT: ");
  Serial.print(pinDHT);
  Serial.print(", Pin LED: ");
  Serial.println(pinLED);
  #endif
}

void SensorTemperatura::actualizar() {
  #ifdef UNIT_TEST
  // ✅ Simular lectura de temperatura
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_real_distribution<float> dis(18.0f, 26.0f);

  float t = dis(gen);
  if (!std::isnan(t)) {
    ultimaTemp = t;
    std::cout << "🌡️ Temperatura: " << t << " °C" << std::endl;

    // Simular control de calefacción
    if (t < tempMin) {
      std::cout << "🔥 Calefacción ACTIVADA" << std::endl;
    } else if (t > tempMax) {
      std::cout << "❄️ Calefacción DESACTIVADA" << std::endl;
    }
  }
  #else
  // ✅ Implementación real
  float t = dht.readTemperature();
  if (!std::isnan(t)) {
    ultimaTemp = t;
    Serial.printf("🌡️ Temperatura: %.2f °C\n", t);

    if (t < tempMin) {
      digitalWrite(ledPin, HIGH);
      Serial.println("🔥 Calefacción ACTIVADA");
    } else if (t > tempMax) {
      digitalWrite(ledPin, LOW);
      Serial.println("❄️ Calefacción DESACTIVADA");
    }
  } else {
    Serial.println("❌ Error leyendo temperatura");
  }
  #endif
}

float SensorTemperatura::getTemperatura() const {
  return ultimaTemp;
}