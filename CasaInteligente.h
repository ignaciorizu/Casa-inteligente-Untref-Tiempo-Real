#ifndef CASA_INTELIGENTE_H
#define CASA_INTELIGENTE_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "SensorLuz.h"
#include "SensorTemperatura.h"
#include "SensorMovimiento.h"
#include "Alarma.h"
#include "PantallaLCD.h"

#define CANT_HABITACIONES 3

// Estado de las habitaciones
struct EstadoPantalla {
  float temp[3];
  int calefaccion[3];
  int lux[3];
  int luz[3];
  String alarmaHab = "";
};

class CasaInteligente {
private:

    // Sensores
    SensorTemperatura* sensoresTemp[CANT_HABITACIONES];
    SensorLuz* sensoresLuz[CANT_HABITACIONES];
    SensorMovimiento* sensoresMov[CANT_HABITACIONES];

    // Componentes principales
    PantallaLCD pantalla;
    Alarma alarma;
    EstadoPantalla estadoPantalla;

    // Queue para botón
    QueueHandle_t botonQueue;

    // Pines
    int ledCalefaccion[CANT_HABITACIONES];
    int ledIluminacion[CANT_HABITACIONES];

public:
    CasaInteligente();

    void iniciar();
    void crearTareas();

    // Tareas (estáticas porque FreeRTOS no acepta métodos normales)
    static void TaskTemp(void* pv);
    static void TaskLight(void* pv);
    static void TaskMotion(void* pv);
    static void TaskAlarm(void* pv);
    static void TaskLCD(void* pv);

    // Acceso global a la instancia
    static CasaInteligente* instancia;

    // ISR
    static void IRAM_ATTR botonISR();

};

#endif
