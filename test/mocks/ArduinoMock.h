#ifndef ARDUINO_MOCK_H
#define ARDUINO_MOCK_H

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

// Definiciones básicas de Arduino
#define HIGH 0x1
#define LOW 0x0
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// Definiciones de interrupciones
#define RISING    0x01
#define FALLING   0x02
#define CHANGE    0x03

#define IRAM_ATTR

// Mock de ESP
class ESPClass {
public:
    void restart();
};

extern ESPClass ESP;

// Mock de Serial
class SerialMock {
public:
    void begin(unsigned long baud);
    void print(const char* text);
    void print(const std::string& text);
    void print(int value);
    void print(float value);
    void println(const char* text);
    void println(const std::string& text);
    void println(int value);
    void println(float value);
    void println(); // Nueva línea solamente
};

// Clase String para testing
class String {
private:
    std::string str;

public:
    String() = default;
    String(const char* s);
    String(const std::string& s);
    String(char c);
    String(int value);
    String(float value, int decimals = 2);

    // Conversiones
    const char* c_str() const { return str.c_str(); }
    operator const char*() const { return str.c_str(); }
    const std::string& std_string() const { return str; }

    // Operaciones comunes
    unsigned int length() const { return str.length(); }
    bool isEmpty() const { return str.empty(); }

    String& operator=(const char* s);
    String& operator=(const std::string& s);
    bool operator==(const char* s) const;
    bool operator==(const std::string& other) const;
    String operator+(const char* s) const;
    String operator+(const std::string& other) const;
    String& operator+=(const char* s);
    String& operator+=(const std::string& other);

    void clear();
    void toLowerCase();
    void toUpperCase();
};

// Declaraciones externas (definidas en mocks.cpp)
extern unsigned long mockMillis;
extern unsigned long mockMicros;
extern std::map<int, int> pinModes;
extern std::map<int, int> pinStates;
extern std::map<int, int> analogValues;
extern std::map<int, int> toneStates;
extern SerialMock Serial;

// Declaraciones de funciones
unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
int analogRead(int pin);
void analogWrite(int pin, int value);
void tone(int pin, int frequency);
void noTone(int pin);

#endif