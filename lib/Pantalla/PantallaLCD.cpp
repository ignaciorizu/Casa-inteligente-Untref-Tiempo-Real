#include "PantallaLCD.h"

#ifdef UNIT_TEST
    #include <iostream>
    #include "../../test/test_common_core.h"  // ✅ Solo incluir
#else
    #include <Arduino.h>
    #include <LiquidCrystal_I2C.h>
    LiquidCrystal_I2C lcd(0x27, 16, 2);
#endif

void PantallaLCD::inicializar() {
    #ifdef UNIT_TEST
    std::cout << "🖥️ Pantalla LCD inicializada (simulada)" << std::endl;
    #else
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sistema Iniciado");
    Serial.println("🖥️ Pantalla LCD inicializada");
    #endif
}

void PantallaLCD::mostrarHabitacion(const String& nombre, float temperatura, bool calefaccion) {
    #ifdef UNIT_TEST
    std::cout << "🖥️ " << nombre.c_str() << " - Temp: " << temperatura << "°C - Calef: " << (calefaccion ? "ON" : "OFF") << std::endl;
    #else
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(nombre);

    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperatura, 1);
    lcd.print("C ");

    if (calefaccion) {
        lcd.print("CALEF");
    }
    #endif
}

void PantallaLCD::mostrarAlarma(const String& zona) {
    #ifdef UNIT_TEST
    std::cout << "🚨 ALARMA en " << zona.c_str() << std::endl;
    #else
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALARMA ACTIVADA");
    lcd.setCursor(0, 1);
    lcd.print("Zona: ");
    lcd.print(zona);
    #endif
}

void PantallaLCD::limpiar() {
    #ifdef UNIT_TEST
    std::cout << "🖥️ Pantalla limpiada" << std::endl;
    #else
    lcd.clear();
    #endif
}