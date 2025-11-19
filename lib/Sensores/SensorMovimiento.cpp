#include "SensorMovimiento.h"

#ifdef UNIT_TEST
    #include <iostream>
    #include <random>
    #include "../../test/test_common_core.h"  // ✅ Solo incluir
#else
    #include <Arduino.h>
#endif

SensorMovimiento::SensorMovimiento(int pinPIR)
    : pin(pinPIR) {

    #ifndef UNIT_TEST
    pinMode(pin, INPUT_PULLUP);
    #endif

    #ifdef UNIT_TEST
    std::cout << "🔧 SensorMovimiento creado (TEST) - Pin: " << pin << std::endl;
    #else
    Serial.print("🔧 SensorMovimiento creado (REAL) - Pin: ");
    Serial.println(pin);
    #endif
}

bool SensorMovimiento::hayMovimiento() {
    bool movimiento = digitalRead(pin);

    #ifdef UNIT_TEST
    if (movimiento) {
        std::cout << "🚨 MOVIMIENTO DETECTADO" << std::endl;
    }
    #else
    if (movimiento) {
        Serial.println("🚨 MOVIMIENTO DETECTADO");
    }
    #endif

    return movimiento;
}