#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// ----- CONFIG ----- //
#define LDR_PIN 32     // luxometro -- 100 >=
#define LED_LIGHT 12   // iluminación --

// ----- FreeRTOS ----- //
QueueHandle_t tempQueue;
QueueHandle_t motionQueue;

// ----- Tarea ----- //
void TaskLight(void *pv);

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(LED_LIGHT, OUTPUT);

  xTaskCreate(TaskLight, "Light", 2048, NULL, 2, NULL);
}

void loop() {}

// ===== TASKS ===== //

void TaskLight(void *pv) {
  while (1) {
    int light = digitalRead(LDR_PIN);
    digitalWrite(LED_LIGHT, light);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
