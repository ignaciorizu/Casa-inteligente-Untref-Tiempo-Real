#ifndef ALARMA_H
#define ALARMA_H

#include <Arduino.h>

class Alarma {
public:
    Alarma(int pinSpeaker, int pinLed);

    void activar();      // Activa sirena y LED
    void desactivar();   // Apaga sirena y LED
    void actualizar();   // Mantiene el estado activo
    bool estaActiva() const;

private:
        int pinSpeaker;
        int pinLed;
        volatile bool activa;
};

#endif
