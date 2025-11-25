#include "mocks/ArduinoMock.h"

// Implementaciones de variables globales
unsigned long mockMillis = 0;
unsigned long mockMicros = 0;

std::map<int, int> pinModes;
std::map<int, int> pinStates;
std::map<int, int> analogValues;
std::map<int, int> toneStates;

SerialMock Serial;
ESPClass ESP;

// Implementaciones de Arduino Mock
void ESPClass::restart() {
    std::cout << "ESP.restart() llamado" << std::endl;
}

// Implementaciones de SerialMock
void SerialMock::begin(unsigned long baud) {
    std::cout << "Serial.begin(" << baud << ")" << std::endl;
}

void SerialMock::print(const char* text) {
    std::cout << text;
}

void SerialMock::print(const std::string& text) {
    std::cout << text;
}

void SerialMock::print(int value) {
    std::cout << value;
}

void SerialMock::print(float value) {
    std::cout << value;
}

void SerialMock::println(const char* text) {
    std::cout << text << std::endl;
}

void SerialMock::println(const std::string& text) {
    std::cout << text << std::endl;
}

void SerialMock::println(int value) {
    std::cout << value << std::endl;
}

void SerialMock::println(float value) {
    std::cout << value << std::endl;
}

void SerialMock::println() {
    std::cout << std::endl;
}

// Implementaciones de String
String::String(const char* s) : str(s ? s : "") {}
String::String(const std::string& s) : str(s) {}
String::String(char c) : str(1, c) {}
String::String(int value) : str(std::to_string(value)) {}
String::String(float value, int decimals) {
    std::ostringstream oss;
    oss.precision(decimals);
    oss << std::fixed << value;
    str = oss.str();
}

String& String::operator=(const char* s) {
    str = s ? s : "";
    return *this;
}

String& String::operator=(const std::string& s) {
    str = s;
    return *this;
}

bool String::operator==(const char* s) const {
    return str == (s ? s : "");
}

bool String::operator==(const std::string& other) const {
    return str == other;
}

String String::operator+(const char* s) const {
    return String(str + (s ? s : ""));
}

String String::operator+(const std::string& other) const {
    return String(str + other);
}

String& String::operator+=(const char* s) {
    str += (s ? s : "");
    return *this;
}

String& String::operator+=(const std::string& other) {
    str += other;
    return *this;
}

void String::clear() {
    str.clear();
}

void String::toLowerCase() {
    for (auto& c : str) {
        c = std::tolower(c);
    }
}

void String::toUpperCase() {
    for (auto& c : str) {
        c = std::toupper(c);
    }
}

// Implementaciones de funciones Arduino
unsigned long millis() {
    return mockMillis;
}

unsigned long micros() {
    return mockMicros;
}

void delay(unsigned long ms) {
    mockMillis += ms;
}

void pinMode(int pin, int mode) {
    pinModes[pin] = mode;
}

void digitalWrite(int pin, int value) {
    pinStates[pin] = value;
}

int digitalRead(int pin) {
    return pinStates.count(pin) ? pinStates[pin] : LOW;
}

int analogRead(int pin) {
    return analogValues.count(pin) ? analogValues[pin] : 0;
}

void analogWrite(int pin, int value) {
    analogValues[pin] = value;
}

void tone(int pin, int frequency) {
    std::cout << "tone(pin=" << pin << ", freq=" << frequency << ")" << std::endl;
    toneStates[pin] = frequency;  // Guardar la frecuencia
    pinStates[pin] = HIGH;        // Marcar el pin como activo
}

void noTone(int pin) {
    std::cout << "noTone(pin=" << pin << ")" << std::endl;
    toneStates.erase(pin);        // Eliminar del tracking
    pinStates[pin] = LOW;         // Marcar el pin como inactivo
}

uint32_t digitalPinToInterrupt(uint32_t pin) {
    return pin;
}

void attachInterrupt(uint32_t pin, void (*isr)(), int mode) {
    // Simular attachInterrupt
}