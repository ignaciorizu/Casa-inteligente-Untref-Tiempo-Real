#ifndef ALARMA_H
#define ALARMA_H

#include "../Interfaces/IAlarma.h"

class Alarma : public IAlarma {
private:
    int pinSpeaker;
    int pinLed;
    bool activa;
    String ultimaZona;

public:
    Alarma(int pinSpeaker, int pinLed);
    void activar(const String& zona) override;
    void desactivar() override;
    void actualizar() override;
    bool estaActiva() const override;
};

#endif