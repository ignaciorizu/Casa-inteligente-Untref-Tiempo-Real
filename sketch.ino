#include <Arduino.h>
#include <DHT.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// ----- CONFIG ----- //
#define DHTPIN 2       // temperatura
#define LDR_PIN 32     // luxometro -- 100 >=
#define LED_LIGHT 12   // iluminación --
#define LED_THERM 27   // termostato

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ----- FreeRTOS ----- //
QueueHandle_t tempQueue;

// ----- Tareas ----- //
void TaskTemp(void *pv);
void TaskLight(void *pv);
void TaskThermostat(void *pv);

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(LED_LIGHT, OUTPUT);
  pinMode(LED_THERM, OUTPUT);

  dht.begin();

  tempQueue = xQueueCreate(5, sizeof(float));

  xTaskCreate(TaskTemp, "Temp", 4096, NULL, 3, NULL);
  xTaskCreate(TaskLight, "Light", 2048, NULL, 2, NULL);
  xTaskCreate(TaskThermostat, "Thermostat", 4096, NULL, 2, NULL);
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
  while (1) {
    int light = digitalRead(LDR_PIN);
    digitalWrite(LED_LIGHT, light);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
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
