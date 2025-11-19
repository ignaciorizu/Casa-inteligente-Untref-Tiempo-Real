#ifndef PANTALLA_LCD_H
#define PANTALLA_LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class PantallaLCD {

private:
    LiquidCrystal_I2C lcd;

public:
    PantallaLCD(uint8_t address = 0x27, uint8_t cols = 20, uint8_t rows = 4);

    void iniciar();
    void limpiar();

    void mostrarInicio();

    void mostrarHabitacion(const String &nombre,
                           float temperatura,
                           bool calefactorEncendido,
                           int lux,
                           bool luzEncendida);

    void mostrarAlarma(const String &zona);
};

#endif
