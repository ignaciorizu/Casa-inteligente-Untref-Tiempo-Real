#ifndef IPANTALLA_LCD_H
#define IPANTALLA_LCD_H

#ifdef UNIT_TEST
    #include <string>

    // En modo test, usar std::string
    typedef std::string String;
#else
    #include <Arduino.h>
    // En modo real, usar String de Arduino
#endif

class IPantallaLCD {
public:
    virtual ~IPantallaLCD() = default;
    virtual void inicializar() = 0;
    virtual void mostrarHabitacion(const String& nombre, float temperatura, bool calefaccion) = 0;
    virtual void mostrarAlarma(const String& zona) = 0;
    virtual void limpiar() = 0;
};

#endif