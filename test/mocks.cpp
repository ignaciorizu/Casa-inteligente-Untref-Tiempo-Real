#include <iostream>
#include <chrono>
#include <thread>
#include <cstdint>

// Mock Arduino functions
void pinMode(int pin, int mode) {}
void digitalWrite(int pin, int value) {}
int digitalRead(int pin) { return 0; }
void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}
unsigned long millis() {
    static auto start = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
}

// Mock Serial
class SerialClass {
public:
    void begin(int baud) {}
    void print(const char* msg) { std::cout << msg; }
    void println(const char* msg) { std::cout << msg << std::endl; }
    void print(int value) { std::cout << value; }
    void println(int value) { std::cout << value << std::endl; }
    void print(float value) { std::cout << value; }
    void println(float value) { std::cout << value << std::endl; }
};

SerialClass Serial;

// Mock DHT (si es necesario)
class DHT {
public:
    DHT(uint8_t pin, uint8_t type) {}
    void begin() {}
    float readTemperature(bool force = false) { return 22.5f; }
    float readHumidity(bool force = false) { return 45.0f; }
};