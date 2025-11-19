#ifndef PANTALLA_LCD_H
#define PANTALLA_LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class PantallaLCD {

private:
    LiquidCrystal_I2C lcd;

public:
    PantallaLCD(uint8_t address = 0x27, uint8_t cols = 16, uint8_t rows = 2);

    void iniciar();
    void limpiar();

    void mostrarInicio();

    void mostrarHabitacion(const String &nombre,
                           float temperatura,
                           bool calefactorEncendido);

    void mostrarAlarma(const String &zona);
};

#endif
