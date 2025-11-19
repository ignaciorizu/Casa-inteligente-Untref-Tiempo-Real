#include "SistemaControl.h"
#include "../Alarma/Alarma.h"
#include "../Pantalla/PantallaLCD.h"

SistemaControl::SistemaControl(
    ISensorLuz** sensoresLuz, int cantLuz,
    ISensorTemperatura** sensoresTemperatura, int cantTemp,
    ISensorMovimiento** sensoresMovimiento, int cantMov,
    IAlarma* alarma, IPantallaLCD* pantalla
) : sensoresLuz(sensoresLuz),
    sensoresTemperatura(sensoresTemperatura),
    sensoresMovimiento(sensoresMovimiento),
    cantidadHabitaciones(cantLuz), // Asumimos que todas las cantidades son iguales
    alarma(alarma),
    pantalla(pantalla) {
}

SistemaControl::~SistemaControl() {
    // Liberar memoria si es necesario
}

void SistemaControl::actualizarSensoresLuz() {
    for (int i = 0; i < cantidadHabitaciones; i++) {
        if (sensoresLuz[i]) {
            // Llamar al método actualizar si existe
            #ifdef UNIT_TEST
            // En tests, simular comportamiento
            #else
            // En código real, llamar a actualizar si el sensor lo tiene
            #endif
        }
    }
}

void SistemaControl::actualizarSensoresTemperatura() {
    for (int i = 0; i < cantidadHabitaciones; i++) {
        if (sensoresTemperatura[i]) {
            sensoresTemperatura[i]->actualizar();
        }
    }
}

void SistemaControl::controlarCalefaccion() {
    for (int i = 0; i < cantidadHabitaciones; i++) {
        if (sensoresTemperatura[i]) {
            float temp = sensoresTemperatura[i]->getTemperatura();
            // Lógica de control de calefacción
            #ifdef UNIT_TEST
            // Simular lógica para tests
            #else
            // Implementación real
            #endif
        }
    }
}

void SistemaControl::verificarMovimiento() {
    bool movimientoDetectado = false;

    for (int i = 0; i < cantidadHabitaciones; i++) {
        if (sensoresMovimiento[i] && sensoresMovimiento[i]->hayMovimiento()) {
            movimientoDetectado = true;
            break;
        }
    }

    if (movimientoDetectado && alarma) {
        alarma->activar("Movimiento detectado");
    }
}

void SistemaControl::actualizarPantalla() {
    if (pantalla) {
        // Lógica para actualizar pantalla
        #ifdef UNIT_TEST
        // Simular para tests
        #else
        // Implementación real
        #endif
    }
}

void SistemaControl::monitorear() {
    actualizarSensoresLuz();
    actualizarSensoresTemperatura();
    controlarCalefaccion();
    verificarMovimiento();
    actualizarPantalla();
}