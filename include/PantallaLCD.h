#ifndef PANTALLA_LCD_H
#define PANTALLA_LCD_H

#ifdef UNIT_TEST
    #include "./test/mocks/ArduinoMock.h"
    #include "./test/mocks/HardwareMock.h"
#else
    #include <Arduino.h>
    #include <LiquidCrystal_I2C.h>
#endif

class PantallaLCD {
  public:
    PantallaLCD(uint8_t address = 0x27, uint8_t cols = 20, uint8_t rows = 4);

    void iniciar();
    void limpiar();
    void mostrarInicio();
    void escribirLinea(int linea, const String &nombre);
    void mostrarHabitacion(const String &nombre,
                           float temperatura,
                           bool calefactorEncendido,
                           int lux,
                           bool luzEncendida);
    void mostrarAlarma(const String &zona);
    void mostrarConfig(const String &titulo, const String &valor);
    void mostrarGuardado();
    void mostrarCargado();

  private:
    LiquidCrystal_I2C lcd;
};

#endif
