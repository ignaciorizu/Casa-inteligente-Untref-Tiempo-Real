#include "Alarma.h"

#ifdef UNIT_TEST
    #include <iostream>
    #include <cstdint>
    #include <chrono>
    #include <thread>
    #include "../../test/test_common_core.h"  // ✅ Ya tiene las definiciones

    // ❌ ELIMINAR TODO ESTO:
    // typedef uint8_t byte;
    // #define HIGH 0x1
    // #define LOW 0x0
    // #define OUTPUT 0x1
    // void pinMode(int pin, int mode) {}
    // void digitalWrite(int pin, int value) {}
    // void delay(unsigned long ms) { ... }
    // unsigned long millis() { ... }
    // class SerialClass { ... };
    // SerialClass Serial;
#else
    #include <Arduino.h>
#endif

Alarma::Alarma(int pinSpeaker, int pinLed)
    : pinSpeaker(pinSpeaker), pinLed(pinLed), activa(false) {

    #ifndef UNIT_TEST
    pinMode(pinSpeaker, OUTPUT);
    pinMode(pinLed, OUTPUT);
    digitalWrite(pinSpeaker, LOW);
    digitalWrite(pinLed, LOW);
    #endif

    #ifdef UNIT_TEST
    std::cout << "🔧 Alarma creada (TEST) - Speaker: " << pinSpeaker << ", LED: " << pinLed << std::endl;
    #else
    Serial.println("🔧 Alarma inicializada");
    #endif
}

void Alarma::activar(const String& zona) {
    activa = true;
    ultimaZona = zona;

    #ifdef UNIT_TEST
    std::cout << "🔔 Alarma ACTIVADA en zona: " << zona.c_str() << std::endl;
    #else
    Serial.print("🔔 Alarma ACTIVADA en zona: ");
    Serial.println(ultimaZona);
    digitalWrite(pinLed, HIGH);
    digitalWrite(pinSpeaker, HIGH);
    #endif
}

void Alarma::desactivar() {
    if (activa) {
        activa = false;
        ultimaZona = "";

        #ifndef UNIT_TEST
        digitalWrite(pinSpeaker, LOW);
        digitalWrite(pinLed, LOW);
        #endif

        #ifdef UNIT_TEST
        std::cout << "🔕 Alarma DESACTIVADA" << std::endl;
        #else
        Serial.println("🔕 Alarma DESACTIVADA");
        #endif
    }
}

void Alarma::actualizar() {
    if (!activa) {
        return;
    }

    #ifdef UNIT_TEST
    // Simular patrón intermitente en tests
    static unsigned long ultimoCambio = 0;
    static bool estadoBuzzer = false;

    unsigned long tiempoActual = millis();

    if (tiempoActual - ultimoCambio >= 500) {
        ultimoCambio = tiempoActual;
        estadoBuzzer = !estadoBuzzer;

        if (estadoBuzzer) {
            std::cout << "🚨 Alarma activa en: " << ultimaZona.c_str() << std::endl;
        }
    }
    #else
    // Implementación real
    static unsigned long ultimoCambio = 0;
    static bool estadoBuzzer = false;

    unsigned long tiempoActual = millis();

    if (tiempoActual - ultimoCambio >= 500) {
        ultimoCambio = tiempoActual;
        estadoBuzzer = !estadoBuzzer;

        digitalWrite(pinSpeaker, estadoBuzzer ? HIGH : LOW);
        digitalWrite(pinLed, HIGH); // LED siempre encendido

        if (estadoBuzzer) {
            Serial.print("🚨 Alarma activa en: ");
            Serial.println(ultimaZona);
        }
    }
    #endif
}

bool Alarma::estaActiva() const {
    return activa;
}