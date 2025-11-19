#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include "../lib/Sensores/SensorLuz.h"
#include "../lib/Sensores/SensorTemperatura.h"
#include "../lib/Sensores/SensorMovimiento.h"
#include "../lib/Alarma/Alarma.h"
#include "../lib/Pantalla/PantallaLCD.h"
#include "../lib/Sistema/SistemaControl.h"

// ----- CONFIGURACIÓN ----- //
#define CANT_HABITACIONES 3

// Sensores de luz
#define LDR_PIN_ENTRADA 32
#define LDR_PIN_PASILLO 35
#define LDR_PIN_SALA 33

#define LED_LIGHT_ENTRADA 12
#define LED_LIGHT_PASILLO 14
#define LED_LIGHT_SALA 13

// Sensores de temperatura (DHT22)
#define DHTPIN_ENTRADA 2
#define DHTPIN_PASILLO 4
#define DHTPIN_SALA 15

// LEDs de calefacción independientes
#define LED_THERM_ENTRADA 27
#define LED_THERM_PASILLO 3
#define LED_THERM_SALA    1

// Sensores PIR
#define PIR_PIN_ENTRADA 19
#define PIR_PIN_PASILLO 21
#define PIR_PIN_SALA    18
// Alarma
#define LED_ALARM 25
#define SPEAKER_PIN 26
#define BOTON_APAGADO_ALARMA 5

// ===== Objetos Globales =====
SensorMovimiento pirEntrada(PIR_PIN_ENTRADA);
SensorMovimiento pirPasillo(PIR_PIN_PASILLO);
SensorMovimiento pirSala(PIR_PIN_SALA);

SensorLuz luzEntrada(LDR_PIN_ENTRADA, LED_LIGHT_ENTRADA, "Entrada");
SensorLuz luzPasillo(LDR_PIN_PASILLO, LED_LIGHT_PASILLO, "Pasillo");
SensorLuz luzSala(LDR_PIN_SALA, LED_LIGHT_SALA, "Sala");

SensorTemperatura tempEntrada(DHTPIN_ENTRADA, LED_THERM_ENTRADA);
SensorTemperatura tempPasillo(DHTPIN_PASILLO, LED_THERM_PASILLO);
SensorTemperatura tempSala(DHTPIN_SALA, LED_THERM_SALA);

IAlarma* alarma = new Alarma(SPEAKER_PIN, LED_ALARM);
IPantallaLCD* pantalla = new PantallaLCD();

// Arrays para el SistemaControl
ISensorMovimiento* sensoresMovimiento[] = {&pirEntrada, &pirPasillo, &pirSala};
ISensorLuz* sensoresLuz[] = {&luzEntrada, &luzPasillo, &luzSala};
ISensorTemperatura* sensoresTemperatura[] = {&tempEntrada, &tempPasillo, &tempSala};

SistemaControl sistema(
    sensoresLuz, CANT_HABITACIONES,
    sensoresTemperatura, CANT_HABITACIONES,
    sensoresMovimiento, CANT_HABITACIONES,
    &alarma, &pantalla
);

QueueHandle_t botonQueue;

// ===== Función ISR =====
void IRAM_ATTR botonISR() {
    int msg = 1;
    xQueueSendFromISR(botonQueue, &msg, NULL);
}

// ===== TASK: SISTEMA PRINCIPAL =====
void TaskSistemaPrincipal(void *pv) {
  while (1) {
    // Actualizar todos los sensores
    sistema.actualizarSensoresLuz();
    sistema.actualizarSensoresTemperatura();

    // Controlar lógica
    sistema.controlarCalefaccion();
    sistema.verificarMovimiento();
    sistema.actualizarPantalla();

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

// ===== TASK: ALARMA =====
void TaskAlarma(void *pv) {
    int msg = 0;
    while (1) {
        if (xQueueReceive(botonQueue, &msg, 0) == pdTRUE) {
            alarma->desactivar();
            pantalla->limpiar();
        }

        alarma->actualizar();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);

  // LEDs de calefacción
  pinMode(LED_THERM_ENTRADA, OUTPUT);
  pinMode(LED_THERM_PASILLO, OUTPUT);
  pinMode(LED_THERM_SALA, OUTPUT);

  // LCD
  Wire.begin(23, 22);  // SDA = 23, SCL = 22

  pantalla->inicializar();
  pantalla->mostrarInicio();
  delay(1500);
  pantalla->limpiar();

  // Botón de apagado de alarma
  botonQueue = xQueueCreate(5, sizeof(int));
  pinMode(BOTON_APAGADO_ALARMA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BOTON_APAGADO_ALARMA), botonISR, FALLING);

  // Creación de tareas FreeRTOS
  xTaskCreate(TaskSistemaPrincipal, "Sistema", 4096, NULL, 3, NULL);
  xTaskCreate(TaskAlarma, "Alarma", 2048, NULL, 2, NULL);
}

void loop() {
  // El loop principal está vacío, todo se maneja en las tasks
}

int main(int argc, char **argv) {
    return 0;
}