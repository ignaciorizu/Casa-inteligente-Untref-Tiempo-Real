#include "PantallaLCD.h"

PantallaLCD::PantallaLCD(uint8_t address, uint8_t cols, uint8_t rows)
    : lcd(address, cols, rows) {}

void PantallaLCD::iniciar() {
    lcd.init();
    lcd.backlight();
    lcd.clear();
}

void PantallaLCD::limpiar() {
    lcd.clear();
}

void PantallaLCD::mostrarInicio() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Sistema Listo");
    lcd.setCursor(0,1);
    lcd.print("Inicializando...");
}

void PantallaLCD::escribirLinea(int linea, const String &nombre) {
    lcd.setCursor(0,linea);
    lcd.print(nombre);
}

void PantallaLCD::mostrarHabitacion(const String &nombre,
                                    float temperatura,
                                    bool calefactorEncendido,
                                    int lux,
                                    bool luzEncendida)
{
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print(nombre);
    lcd.print(": ");
    lcd.print(temperatura, 1);
    lcd.print("C");

    lcd.setCursor(0,1);
    lcd.print("Calefactor: ");
    lcd.print(calefactorEncendido ? "ON" : "OFF");

    lcd.setCursor(0,2);
    lcd.print("Iluminacion: ");
    lcd.print(lux);
    lcd.print(" lux");
    
    lcd.setCursor(0,3);
    lcd.print("Luz: ");
    lcd.print(luzEncendida ? "ON" : "OFF");
}

void PantallaLCD::mostrarAlarma(const String &zona) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("!!! ALARMA !!!");

    lcd.setCursor(0,1);
    lcd.print(zona);
}

void PantallaLCD::mostrarConfig(const String &titulo, const String &valor) {
    lcd.clear();

    lcd.setCursor(0,0);
    lcd.print("CONFIG:");

    lcd.setCursor(0,1);
    lcd.print(titulo);

    lcd.setCursor(0,2);
    lcd.print("Valor:");

    lcd.setCursor(0,3);
    lcd.print(valor);
}
