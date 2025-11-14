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

#define LDR_PIN_ENTRADA 32     // Sensor de Luz - Entrada
#define LDR_PIN_PASILLO 35     // Sensor de Luz - Pasillo
#define LDR_PIN_SALA 33        // Sensor de Luz - Sala

#define LED_LIGHT_ENTRADA 12   // Iluminación - Entrada
#define LED_LIGHT_PASILLO 14   // Iluminación - Pasillo
#define LED_LIGHT_SALA 13      // Iluminación - Sala

#define DHTPIN_ENTRADA 2    // Sensor de Temperatura - Entrada
#define DHTPIN_PASILLO 4     // Sensor de Temperatura - Pasillo
#define DHTPIN_SALA 15        // Sensor de Temperatura - Sala

#define LED_THERM 27   // Termostato General

#define PIR_PIN_ENTRADA 19      // Sensor de Movimiento - Entrada
#define PIR_PIN_PASILLO 21      // Sensor de Movimiento - Pasillo
#define PIR_PIN_SALA 18      // Sensor de Movimiento - Sala

#define LED_ALARM 25    // Alarma
#define SPEAKER_PIN 26  // Sirena de alarma
#define BOTON_APAGADO_ALARMA 5 // Parada de alarma

// ===== GLOBAL =====
SensorMovimiento pirEntrada(PIR_PIN_ENTRADA);
SensorMovimiento pirPasillo(PIR_PIN_PASILLO);
SensorMovimiento pirSala(PIR_PIN_SALA);
Alarma alarma(SPEAKER_PIN, LED_ALARM);

// ----- FreeRTOS ----- //
QueueHandle_t botonQueue;

// ----- Funciones auxiliares ----- //
void IRAM_ATTR botonISR();
float leerTemperaturas(SensorTemperatura sensores[], int cantidad);
void controlarCalefaccion(float promedio);

// ----- Tareas ----- //
void TaskTemp(void *pv);
void TaskLight(void *pv);
void TaskMotion(void *pv);
void TaskAlarm(void *pv);

void setup() {
  Serial.begin(115200);
  
  pinMode(LED_THERM, OUTPUT);

  botonQueue  = xQueueCreate(5, sizeof(int));
  
  pinMode(BOTON_APAGADO_ALARMA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTON_APAGADO_ALARMA), botonISR, FALLING);

  xTaskCreate(TaskTemp, "Temp", 4096, NULL, 3, NULL);
  xTaskCreate(TaskLight, "Light", 2048, NULL, 2, NULL);
  xTaskCreate(TaskMotion, "Motion", 2048, NULL, 2, NULL);
  xTaskCreate(TaskAlarm, "Alarm", 2048, NULL, 2, NULL);
}

void loop() {}

// ===== FUNCIONES AUXILIARES ===== //

void IRAM_ATTR botonISR() {
    int msg = 1;
    xQueueSendFromISR(botonQueue, &msg, NULL);
}

float leerTemperaturas(SensorTemperatura sensores[], int cantidad) {
  float suma = 0;
  int count = 0;

  for (int i = 0; i < cantidad; i++) {
    sensores[i].actualizar();
    float t = sensores[i].getTemperatura();
    if (!isnan(t)) {
      suma += t;
      count++;
    }
  }

  if (count == 0) return NAN;
  float promedio = suma / count;
  Serial.printf("Temperatura promedio: %.2f °C\n", promedio);
  return promedio;
}

void controlarCalefaccion(float promedio) {

  if (isnan(promedio)) return;

  if (promedio < 20.0) {
    digitalWrite(LED_THERM, HIGH);
  } 
  else if (promedio > 24.0) {
    digitalWrite(LED_THERM, LOW);
  }
}

// ===== TASKS ===== //

void TaskTemp(void *pv) {
  SensorTemperatura sensores[CANT_HABITACIONES] = {
    SensorTemperatura(DHTPIN_ENTRADA, LED_THERM),
    SensorTemperatura(DHTPIN_PASILLO, LED_THERM),
    SensorTemperatura(DHTPIN_SALA, LED_THERM)
  };

  while (1) {
    float promedio = leerTemperaturas(sensores, CANT_HABITACIONES);
    controlarCalefaccion(promedio);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }
}


void TaskLight(void *pv) {
  SensorLuz luces[CANT_HABITACIONES]= {
     SensorLuz(LDR_PIN_ENTRADA, LED_LIGHT_ENTRADA),
     SensorLuz(LDR_PIN_PASILLO, LED_LIGHT_PASILLO),
     SensorLuz(LDR_PIN_SALA, LED_LIGHT_SALA)
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

        if (pirEntrada.hayMovimiento() ||
            pirPasillo.hayMovimiento() ||
            pirSala.hayMovimiento()) 
        {
            alarma.activar();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
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
