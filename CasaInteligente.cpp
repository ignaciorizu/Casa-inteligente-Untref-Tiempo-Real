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

//IR
#define DATA_IR 16


// ========== Instancia global estática ==========
CasaInteligente* CasaInteligente::instancia = nullptr;


// ==================================================
//                Constructor
// ==================================================
CasaInteligente::CasaInteligente()
    : alarma(SPEAKER_PIN, LED_ALARM), ir(DATA_IR) 
{
    instancia = this;
    
    config = new ConfigManager();

    // Crear sensores temperatura
    sensoresTemp[0] = new SensorTemperatura(DHT_ENTRADA, LED_THERM_ENTRADA);
    sensoresTemp[1] = new SensorTemperatura(DHT_PASILLO, LED_THERM_PASILLO);
    sensoresTemp[2] = new SensorTemperatura(DHT_SALA, LED_THERM_SALA);

    ledCalefaccion[0] = LED_THERM_ENTRADA;
    ledCalefaccion[1] = LED_THERM_PASILLO;
    ledCalefaccion[2] = LED_THERM_SALA;

    // Crear sensores luz
    sensoresLuz[0] = new SensorLuz(LDR_ENTRADA, LED_LIGHT_ENTRADA, "Entrada", *config);
    sensoresLuz[1] = new SensorLuz(LDR_PASILLO, LED_LIGHT_PASILLO, "Pasillo", *config);
    sensoresLuz[2] = new SensorLuz(LDR_SALA, LED_LIGHT_SALA, "Sala", *config);

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

    // IR
    ir.begin();

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
    xTaskCreate(TaskLCD, "LCD", 4096, NULL, 2, NULL);
    xTaskCreate(TaskIR, "IR", 2048, NULL, 1, NULL);
}


// ==================================================
//                TASK: LCD
// ==================================================
void CasaInteligente::TaskLCD(void* pv) {
    auto* casa = CasaInteligente::instancia;

    int indice = 0;

    while (1) {

        if (casa->alarma.estaActiva()) {
            casa->pantalla.mostrarAlarma(casa->estadoPantalla.alarmaHab);
            vTaskDelay(1500 / portTICK_PERIOD_MS);
            continue;
        }
        
        if (casa->config->enMenu()) {
            casa->pantalla.limpiar();
            casa->pantalla.escribirLinea(0, casa->config->getTextoOpcion());
            casa->pantalla.escribirLinea(1, String(casa->config->getValorActual()));
            vTaskDelay(350);
            continue;
        }

        const char* nombres[] = {"Entrada", "Pasillo", "Sala"};

        casa->pantalla.mostrarHabitacion(
            nombres[indice],
            casa->estadoPantalla.temp[indice],
            casa->estadoPantalla.calefaccion[indice],
            casa->estadoPantalla.lux[indice],
            casa->estadoPantalla.luz[indice]
        );

        indice = (indice + 1) % 3;

        vTaskDelay(200  / portTICK_PERIOD_MS);
    }
}


// ==================================================
//                TASK: IR
// ==================================================
void CasaInteligente::TaskIR(void* pv) {
    auto* casa = CasaInteligente::instancia;

    while (1) {

        if (casa->ir.available()) {
            uint32_t code = casa->ir.getLastCode();
            casa->ir.resume();

            IRButton boton = decodeButton(code);

            if (boton == IR_POWER) {
                casa->config->setModoConfig(!casa->config->enMenu());
            }

            if (casa->config->enMenu()) {
                casa->config->procesarCodigoIR(boton);
            }
        }
        vTaskDelay(80 / portTICK_PERIOD_MS);
    }
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

            casa->estadoPantalla.temp[i] = t;
            casa->estadoPantalla.calefaccion[i] = digitalRead(casa->ledCalefaccion[i]);
        }

        // Si la alarma está activa, no mostrar temperatura
        if (casa->alarma.estaActiva()) {
            vTaskDelay(500 / portTICK_PERIOD_MS);
            continue;
        }

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
            casa->estadoPantalla.lux[i] = casa->sensoresLuz[i]->getLux();
            casa->estadoPantalla.luz[i] = digitalRead(casa->ledIluminacion[i]);
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
            casa->estadoPantalla.alarmaHab = "Entrada";
        }

        if (casa->sensoresMov[1]->hayMovimiento()) {
            casa->alarma.activar("Pasillo");
            casa->estadoPantalla.alarmaHab = "Pasillo";
        }

        if (casa->sensoresMov[2]->hayMovimiento()) {
            casa->alarma.activar("Sala");
            casa->estadoPantalla.alarmaHab = "Sala";
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
