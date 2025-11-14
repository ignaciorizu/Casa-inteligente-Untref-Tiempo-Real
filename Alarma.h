#ifndef ALARMA_H
#define ALARMA_H

#include <Arduino.h>

class Alarma {
private:
    int buzzerPin;
    int ledPin;
    int botonPin;
    int estado;  // 0 = OFF, 1 = ON

public:
    Alarma(int buz, int led, int btn);

    void activar();
    void apagar();
    void chequearBoton();
    int getEstado() const;
};

#endif
