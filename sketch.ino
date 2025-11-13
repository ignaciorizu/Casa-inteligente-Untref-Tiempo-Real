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
#define PIR_PIN 19     // movimiento --
#define LED_ALARM 25   // alarma / sirena --
#define SPEAKER_PIN 26  // sonido de alarma

#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// ----- FreeRTOS ----- //
QueueHandle_t tempQueue;
QueueHandle_t motionQueue;

// ----- Tareas ----- //
void TaskTemp(void *pv);
void TaskLight(void *pv);
void TaskThermostat(void *pv);
void TaskMotion(void *pv);
void TaskAlarm(void *pv);

void setup() {
  Serial.begin(115200);

  pinMode(LDR_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_LIGHT, OUTPUT);
  pinMode(LED_THERM, OUTPUT);
  pinMode(LED_ALARM, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);

  dht.begin();

  tempQueue = xQueueCreate(5, sizeof(float));
  motionQueue = xQueueCreate(5, sizeof(int));

  xTaskCreate(TaskTemp, "Temp", 4096, NULL, 3, NULL);
  xTaskCreate(TaskLight, "Light", 2048, NULL, 2, NULL);
  xTaskCreate(TaskThermostat, "Thermostat", 4096, NULL, 2, NULL);
  xTaskCreate(TaskMotion, "Motion", 2048, NULL, 3, NULL);
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

void TaskMotion(void *pv) {
  while (1) {
    if (digitalRead(PIR_PIN)) {
      int alert = 1;
      xQueueSend(motionQueue, &alert, 0);
    }
    vTaskDelay(300 / portTICK_PERIOD_MS);
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
