#include "SensorLuz.h"

// Incluir Arduino.h solo cuando no estamos en modo test
#ifdef UNIT_TEST
    #include "../../test/test_common_core.h"  // ✅ Sin gtest dependency
#else
    #include <Arduino.h>
#endif

SensorLuz::SensorLuz(int pinSensor, int pinLed, const char* ubicacion)
    : pinSensor(pinSensor), pinLed(pinLed), ubicacion(ubicacion), lux(0) {

#ifndef UNIT_TEST
    pinMode(pinLed, OUTPUT);
#endif
}

int SensorLuz::leerLuz() {
#ifdef UNIT_TEST
    // Mock para testing - valor simulado
    return 1000;
#else
    return analogRead(pinSensor);
#endif
}

void SensorLuz::encenderLuz() {
#ifndef UNIT_TEST
    digitalWrite(pinLed, HIGH);
#endif
}

void SensorLuz::apagarLuz() {
#ifndef UNIT_TEST
    digitalWrite(pinLed, LOW);
#endif
}

void SensorLuz::actualizar() {
    int lectura = leerLuz();
    // Simular conversión a lux (algoritmo simplificado)
    lux = lectura * 0.1f;
}

const char* SensorLuz::getUbicacion() {
    return ubicacion;
}