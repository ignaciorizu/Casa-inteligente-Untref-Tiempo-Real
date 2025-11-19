#pragma once

class ISensorLuz {
public:
    virtual ~ISensorLuz() = default;
    virtual int leerLuz() = 0;
    virtual void encenderLuz() = 0;
    virtual void apagarLuz() = 0;
    virtual const char* getUbicacion() = 0;
    virtual void actualizar() = 0;                    // ✅ AÑADIR
    virtual float getLux() const = 0;                 // ✅ AÑADIR
};