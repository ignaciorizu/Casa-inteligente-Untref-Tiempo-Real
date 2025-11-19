#pragma once

#include "../Interfaces/ISistemaControl.h"
#include "../Interfaces/ISensorLuz.h"
#include "../Interfaces/ISensorTemperatura.h"
#include "../Interfaces/ISensorMovimiento.h"
#include "../Interfaces/IAlarma.h"
#include "../Interfaces/IPantallaLCD.h"

// Forward declarations para evitar includes circulares
class Alarma;
class PantallaLCD;

class SistemaControl  : public ISistemaControl {
private:
    ISensorLuz** sensoresLuz;
    ISensorTemperatura** sensoresTemperatura;
    ISensorMovimiento** sensoresMovimiento;
    int cantidadHabitaciones;
    IAlarma* alarma;
    IPantallaLCD* pantalla;

public:
    SistemaControl(
        ISensorLuz** sensoresLuz, int cantLuz,
        ISensorTemperatura** sensoresTemperatura, int cantTemp,
        ISensorMovimiento** sensoresMovimiento, int cantMov,
        IAlarma* alarma, IPantallaLCD* pantalla
    );

    // Destructor
    ~SistemaControl();

    // Métodos de control
    void actualizarSensoresLuz();
    void actualizarSensoresTemperatura();
    void controlarCalefaccion();
    void verificarMovimiento();
    void actualizarPantalla();
    void monitorear();

    // Getters para tests
    IAlarma* getAlarma() const { return alarma; }
    IPantallaLCD* getPantalla() const { return pantalla; }
    int getCantidadHabitaciones() const { return cantidadHabitaciones; }
};