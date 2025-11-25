#ifndef TEST_COMMON_H
#define TEST_COMMON_H

// Configuración común para tests
#ifdef UNIT_TEST
    // Usar nuestra clase String del mock
    #include "mocks/ArduinoMock.h"
    #include "mocks/IRremoteMock.h"
#else
    #include <Arduino.h>
#endif

#include <string>
#include <cmath>

// Helper functions para testing
class TestHelper {
public:
    static void resetArduinoMock();
    static void advanceTime(unsigned long ms);
    static void setPinState(int pin, int state);
    static int getPinState(int pin);
};

// Implementación de TestHelper
inline void TestHelper::resetArduinoMock() {
    mockMillis = 0;
    mockMicros = 0;
    pinModes.clear();
    pinStates.clear();
    analogValues.clear();
    toneStates.clear();

    #ifdef UNIT_TEST
    // Resetear IR mock - CORREGIDO: usar el nombre correcto
    IrReceiverMock.reset();  // Usar el método reset que creamos
    #endif
}

inline void TestHelper::advanceTime(unsigned long ms) {
    mockMillis += ms;
    mockMicros += ms * 1000;
}

inline void TestHelper::setPinState(int pin, int state) {
    pinStates[pin] = state;
}

inline int TestHelper::getPinState(int pin) {
    return pinStates.count(pin) ? pinStates[pin] : LOW;
}

#endif