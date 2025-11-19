#ifndef PANTALLA_LCD_H
#define PANTALLA_LCD_H

#include "../Interfaces/IPantallaLCD.h"

class PantallaLCD : public IPantallaLCD {
public:
    PantallaLCD() = default;
    void inicializar() override;
    void mostrarHabitacion(const String& nombre, float temperatura, bool calefaccion) override;
    void mostrarAlarma(const String& zona) override;
    void limpiar() override;
};

#endif