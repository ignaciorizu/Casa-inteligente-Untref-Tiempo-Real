#ifndef HARDWARE_MOCK_H
#define HARDWARE_MOCK_H

#include "ArduinoMock.h"
#include "IRremoteMock.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <cmath>

// LiquidCrystal_I2C Mock
class LiquidCrystal_I2C {
private:
    uint8_t address;
    uint8_t cols;
    uint8_t rows;
    std::string buffer[4];

public:
    LiquidCrystal_I2C(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
        : address(lcd_addr), cols(lcd_cols), rows(lcd_rows) {}

    void init() {
        std::cout << "LCD inicializado: " << cols << "x" << rows << " @ 0x" << std::hex << (int)address << std::dec << std::endl;
    }

    void backlight() {}
    void clear() {
        for(int i = 0; i < 4; i++) buffer[i].clear();
    }

    void setCursor(uint8_t col, uint8_t row) {
        // Simular cursor
    }

    void print(const char* str) {
        if (str) {
            // Simular impresión en buffer (simplificado)
        }
    }

    void print(const std::string& str) {
        print(str.c_str());
    }

    void print(float value, int decimals) {
        // Simular impresión de float
    }

    void print(int value) {
        // Simular impresión de int
    }

    // Para testing
    std::string getLine(int line) const {
        if(line >= 0 && line < 4) return buffer[line];
        return "";
    }
};

#endif