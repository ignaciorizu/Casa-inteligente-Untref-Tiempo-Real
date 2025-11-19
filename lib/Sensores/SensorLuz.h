#pragma once
#include "../Interfaces/ISensorLuz.h"

class SensorLuz : public ISensorLuz {
private:
    int pinSensor;
    int pinLed;
    const char* ubicacion;
    float lux;

public:
    SensorLuz(int pinSensor, int pinLed, const char* ubicacion);

    // Métodos de la interfaz ISensorLuz
    int leerLuz() override;
    void encenderLuz() override;
    void apagarLuz() override;
    const char* getUbicacion() override;
    void actualizar() override;                       // ✅ AÑADIR override
    float getLux() const override { return lux; }     // ✅ AÑADIR override

    // ❌ ELIMINAR métodos adicionales (ya están en la interfaz)
    // void actualizar();  ← QUITAR
    // float getLux() const { return lux; } ← QUITAR (ya está arriba con override)
};