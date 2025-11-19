#include "CasaInteligente.h"

// ====== ASIGNACIÓN DE PINES ======

// Luz
#define LDR_ENTRADA 32
#define LDR_PASILLO 35
#define LDR_SALA    33

#define LED_LIGHT_ENTRADA 12
#define LED_LIGHT_PASILLO 14
#define LED_LIGHT_SALA    13

// Temperatura
#define DHT_ENTRADA 2
#define DHT_PASILLO 4
#define DHT_SALA    15

#define LED_THERM_ENTRADA 27
#define LED_THERM_PASILLO 3
#define LED_THERM_SALA    1

// Movimiento
#define PIR_ENTRADA 19
#define PIR_PASILLO 21
#define PIR_SALA    18

// Alarma
#define LED_ALARM 25
#define SPEAKER_PIN 26
#define BOTON_APAGADO_ALARMA 5


// ========== Instancia global estática ==========
CasaInteligente* CasaInteligente::instancia = nullptr;


// ==================================================
//                Constructor
// ==================================================
CasaInteligente::CasaInteligente()
    : alarma(SPEAKER_PIN, LED_ALARM) 
{
    instancia = this;

    // Crear sensores temperatura
    sensoresTemp[0] = new SensorTemperatura(DHT_ENTRADA, LED_THERM_ENTRADA);
    sensoresTemp[1] = new SensorTemperatura(DHT_PASILLO, LED_THERM_PASILLO);
    sensoresTemp[2] = new SensorTemperatura(DHT_SALA, LED_THERM_SALA);

    ledCalefaccion[0] = LED_THERM_ENTRADA;
    ledCalefaccion[1] = LED_THERM_PASILLO;
    ledCalefaccion[2] = LED_THERM_SALA;

    // Crear sensores luz
    sensoresLuz[0] = new SensorLuz(LDR_ENTRADA, LED_LIGHT_ENTRADA, "Entrada");
    sensoresLuz[1] = new SensorLuz(LDR_PASILLO, LED_LIGHT_PASILLO, "Pasillo");
    sensoresLuz[2] = new SensorLuz(LDR_SALA, LED_LIGHT_SALA, "Sala");

    ledIluminacion[0] = LED_LIGHT_ENTRADA;
    ledIluminacion[1] = LED_LIGHT_PASILLO;
    ledIluminacion[2] = LED_LIGHT_SALA;

    // Crear PIR
    sensoresMov[0] = new SensorMovimiento(PIR_ENTRADA);
    sensoresMov[1] = new SensorMovimiento(PIR_PASILLO);
    sensoresMov[2] = new SensorMovimiento(PIR_SALA);
}


// ==================================================
//                 INICIAR EL SISTEMA
// ==================================================
void CasaInteligente::iniciar() {

    // LEDs calefacción
    for (int i = 0; i < CANT_HABITACIONES; i++) {
        pinMode(ledCalefaccion[i], OUTPUT);
    }

    // LCD
    Wire.begin(23, 22);
    pantalla.iniciar();
    pantalla.mostrarInicio();
    delay(1500);
    pantalla.limpiar();

    // Botón alarma
    botonQueue = xQueueCreate(5, sizeof(int));
    pinMode(BOTON_APAGADO_ALARMA, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BOTON_APAGADO_ALARMA), botonISR, FALLING);
}


// ISR estática
void IRAM_ATTR CasaInteligente::botonISR() {
    int msg = 1;
    xQueueSendFromISR(instancia->botonQueue, &msg, NULL);
}


// ==================================================
//                  CREAR TAREAS
// ==================================================
void CasaInteligente::crearTareas() {
    xTaskCreate(TaskTemp,   "Temp",   4096, NULL, 3, NULL);
    xTaskCreate(TaskLight,  "Light",  2048, NULL, 2, NULL);
    xTaskCreate(TaskMotion, "Motion", 2048, NULL, 2, NULL);
    xTaskCreate(TaskAlarm,  "Alarm",  2048, NULL, 2, NULL);
}


// ==================================================
//                TASK: TEMPERATURA
// ==================================================
void CasaInteligente::TaskTemp(void* pv) {

    auto* casa = CasaInteligente::instancia;

    while (1) {

        // Leer sensores
        for (int i = 0; i < CANT_HABITACIONES; i++) {
            casa->sensoresTemp[i]->actualizar();

            float t = casa->sensoresTemp[i]->getTemperatura();
            int led = casa->ledCalefaccion[i];

            if (!isnan(t)) {
                if (t < 20.0) digitalWrite(led, HIGH);
                else if (t > 24.0) digitalWrite(led, LOW);
            }
        }

        // Si la alarma está activa, no mostrar temperatura
        if (casa->alarma.estaActiva()) {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }

        // Mostrar una habitación por ciclo
        casa->pantalla.mostrarHabitacion("Entrada",
                casa->sensoresTemp[0]->getTemperatura(),
                digitalRead(casa->ledCalefaccion[0]),
                casa->sensoresLuz[0]->getLux(),
                digitalRead(casa->ledIluminacion[0]));
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        if (casa->alarma.estaActiva()) continue;

        casa->pantalla.mostrarHabitacion("Pasillo",
                casa->sensoresTemp[1]->getTemperatura(),
                digitalRead(casa->ledCalefaccion[1]),
                casa->sensoresLuz[1]->getLux(),
                digitalRead(casa->ledIluminacion[1]));
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        if (casa->alarma.estaActiva()) continue;

        casa->pantalla.mostrarHabitacion("Sala",
                casa->sensoresTemp[2]->getTemperatura(),
                digitalRead(casa->ledCalefaccion[2]),
                casa->sensoresLuz[2]->getLux(),
                digitalRead(casa->ledIluminacion[2]));
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}


// ==================================================
//                TASK: LUZ
// ==================================================
void CasaInteligente::TaskLight(void* pv) {
    auto* casa = CasaInteligente::instancia;

    while (1) {
        for (int i = 0; i < CANT_HABITACIONES; i++) {
            casa->sensoresLuz[i]->actualizar();
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


// ==================================================
//               TASK: MOVIMIENTO
// ==================================================
void CasaInteligente::TaskMotion(void* pv) {
    auto* casa = CasaInteligente::instancia;

    while (1) {

        if (casa->sensoresMov[0]->hayMovimiento()) {
            casa->alarma.activar("Entrada");
            casa->pantalla.mostrarAlarma("Entrada");
        }

        if (casa->sensoresMov[1]->hayMovimiento()) {
            casa->alarma.activar("Pasillo");
            casa->pantalla.mostrarAlarma("Pasillo");
        }

        if (casa->sensoresMov[2]->hayMovimiento()) {
            casa->alarma.activar("Sala");
            casa->pantalla.mostrarAlarma("Sala");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


// ==================================================
//                TASK: ALARMA
// ==================================================
void CasaInteligente::TaskAlarm(void* pv) {
    auto* casa = CasaInteligente::instancia;

    int msg = 0;

    while (1) {
        if (xQueueReceive(casa->botonQueue, &msg, 0) == pdTRUE) {
            casa->alarma.desactivar();
            casa->pantalla.limpiar();
        }

        casa->alarma.actualizar();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}
