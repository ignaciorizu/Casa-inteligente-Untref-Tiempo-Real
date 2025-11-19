#ifndef ISISTEMA_CONTROL_H
#define ISISTEMA_CONTROL_H

#include "ISensorLuz.h"
#include "ISensorTemperatura.h"
#include "ISensorMovimiento.h"
#include "IAlarma.h"
#include "IPantallaLCD.h"

class ISistemaControl {
public:
    virtual ~ISistemaControl() = default;
    virtual void actualizarSensoresLuz() = 0;
    virtual void actualizarSensoresTemperatura() = 0;
    virtual void verificarMovimiento() = 0;
    virtual void controlarCalefaccion() = 0;
    virtual void actualizarPantalla() = 0;
    virtual void monitorear() = 0;

    virtual IAlarma* getAlarma() const = 0;
    virtual IPantallaLCD* getPantalla() const = 0;
};

#endif