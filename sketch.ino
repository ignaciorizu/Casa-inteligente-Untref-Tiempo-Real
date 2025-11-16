#include <Arduino.h>
#include "SensorLuz.h"
#include "SensorTemperatura.h"
#include "SensorMovimiento.h"
#include "Alarma.h"
#include <DHT.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// ----- CONFIG ----- //

#define CANT_HABITACIONES 3  

#define LDR_PIN_ENTRADA 32
#define LDR_PIN_PASILLO 35
#define LDR_PIN_SALA 33

#define LED_LIGHT_ENTRADA 12
#define LED_LIGHT_PASILLO 14
#define LED_LIGHT_SALA 13

#define DHTPIN_ENTRADA 2
#define DHTPIN_PASILLO 4
#define DHTPIN_SALA 15

#define LED_THERM_ENTRADA 27
#define LED_THERM_PASILLO 3
#define LED_THERM_SALA    1

#define PIR_PIN_ENTRADA 19
#define PIR_PIN_PASILLO 21
#define PIR_PIN_SALA 18

#define LED_ALARM 25
#define SPEAKER_PIN 26
#define BOTON_APAGADO_ALARMA 5

// ===== GLOBAL =====
SensorMovimiento pirEntrada(PIR_PIN_ENTRADA);
SensorMovimiento pirPasillo(PIR_PIN_PASILLO);
SensorMovimiento pirSala(PIR_PIN_SALA);
Alarma alarma(SPEAKER_PIN, LED_ALARM);

QueueHandle_t botonQueue;

// ===== Funciones auxiliares ===== //
void IRAM_ATTR botonISR();

// -----------------------------------------
// Nueva función individual por sensor
// -----------------------------------------
void controlarCalefaccionIndividual(SensorTemperatura sensores[], int cantidad) {
  for (int i = 0; i < cantidad; i++) {

    float t = sensores[i].getTemperatura();
    int led = sensores[i].getLedPin();

    if (isnan(t)) continue;

    if (t < 20.0)
        digitalWrite(led, HIGH);
    else if (t > 24.0)
        digitalWrite(led, LOW);
  }
}

// ===== TAREAS ===== //

void TaskTemp(void *pv) {
  SensorTemperatura sensores[CANT_HABITACIONES] = {
    SensorTemperatura(DHTPIN_ENTRADA, LED_THERM_ENTRADA),
    SensorTemperatura(DHTPIN_PASILLO, LED_THERM_PASILLO),
    SensorTemperatura(DHTPIN_SALA, LED_THERM_SALA)
  };

  while (1) {
    
    // Leer temperatura de cada sensor
    for (int i = 0; i < CANT_HABITACIONES; i++) {
      sensores[i].actualizar();
    }

    // Controlar calefacción por sensor
    controlarCalefaccionIndividual(sensores, CANT_HABITACIONES);

    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}

void TaskLight(void *pv) {
  SensorLuz luces[CANT_HABITACIONES]= {
     SensorLuz(LDR_PIN_ENTRADA, LED_LIGHT_ENTRADA, "Entrada"),
     SensorLuz(LDR_PIN_PASILLO, LED_LIGHT_PASILLO, "Pasillo"),
     SensorLuz(LDR_PIN_SALA, LED_LIGHT_SALA, "Sala")
  };

  while (1) {
    for (auto &l : luces) {
      l.actualizar();
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskMotion(void *pv) {
  while (1) {
    if (pirEntrada.hayMovimiento())
      alarma.activar("Entrada");
    if (pirPasillo.hayMovimiento())
      alarma.activar("Pasillo");
    if (pirSala.hayMovimiento())
      alarma.activar("Sala");

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void TaskAlarm(void *pv) {
    int msg = 0;
    while (1) {
        if (xQueueReceive(botonQueue, &msg, 0) == pdTRUE) {
            alarma.desactivar();
        }

        alarma.actualizar();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void setup() {
  Serial.begin(115200);

  // Configurar LEDs individuales
  pinMode(LED_THERM_ENTRADA, OUTPUT);
  pinMode(LED_THERM_PASILLO, OUTPUT);
  pinMode(LED_THERM_SALA, OUTPUT);

  botonQueue  = xQueueCreate(5, sizeof(int));

  pinMode(BOTON_APAGADO_ALARMA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTON_APAGADO_ALARMA), botonISR, FALLING);

  xTaskCreate(TaskTemp,   "Temp",   4096, NULL, 3, NULL);
  xTaskCreate(TaskLight,  "Light",  2048, NULL, 2, NULL);
  xTaskCreate(TaskMotion, "Motion", 2048, NULL, 2, NULL);
  xTaskCreate(TaskAlarm,  "Alarm",  2048, NULL, 2, NULL);
}

void loop() {}

void IRAM_ATTR botonISR() {
    int msg = 1;
    xQueueSendFromISR(botonQueue, &msg, NULL);
}
