#include <Arduino.h>
#include "SensorLuz.h"
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

#define DHTPIN 2        // Sensor de Temperatura
#define PIR_PIN 19      // Sensor de Movimiento
#define LED_THERM 27    // Calefaccion
#define LED_ALARM 25    // Alarma
#define SPEAKER_PIN 26  // Sirena de alarma

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ----- FreeRTOS ----- //
QueueHandle_t tempQueue;
QueueHandle_t motionQueue;

// ----- Tareas ----- //
void TaskTemp(void *pv);
void TaskLight(void *pv);
void TaskMotion(void *pv);
void TaskThermostat(void *pv);
void TaskAlarm(void *pv);

void setup() {
  Serial.begin(115200);
  
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_THERM, OUTPUT);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  dht.begin();

  tempQueue = xQueueCreate(5, sizeof(float));
  motionQueue = xQueueCreate(5, sizeof(int));

  xTaskCreate(TaskTemp, "Temp", 4096, NULL, 3, NULL);
  xTaskCreate(TaskLight, "Light", 2048, NULL, 2, NULL);
  xTaskCreate(TaskMotion, "Motion", 2048, NULL, 3, NULL);
  xTaskCreate(TaskThermostat, "Thermostat", 4096, NULL, 2, NULL);
  xTaskCreate(TaskAlarm, "Alarm", 2048, NULL, 2, NULL);
}

void loop() {}

// ===== TASKS ===== //

void TaskTemp(void *pv) {
  while (1) {
    float t = dht.readTemperature();
    if (!isnan(t)) {
      xQueueSend(tempQueue, &t, 0);
      Serial.printf("Temp: %.2f C\n", t);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
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
    if (digitalRead(PIR_PIN)) {
      int alert = 1;
      xQueueSend(motionQueue, &alert, 0);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void TaskThermostat(void *pv) {
  float temp;
  while (1) {
    if (xQueueReceive(tempQueue, &temp, portMAX_DELAY)) {
      if (temp < 20) digitalWrite(LED_THERM, HIGH);
      else if (temp > 24) digitalWrite(LED_THERM, LOW);
    }
  }
}

void TaskAlarm(void *pv) {
  int alert;
  while (1) {
    if (xQueueReceive(motionQueue, &alert, portMAX_DELAY)) {
      Serial.println("⚠ Movimiento detectado! ALARMA!");
      digitalWrite(LED_ALARM, HIGH);
      
      tone(SPEAKER_PIN,  262, 250);
      vTaskDelay(2000 / portTICK_PERIOD_MS);
      digitalWrite(LED_ALARM, LOW);
    }
  }
}
