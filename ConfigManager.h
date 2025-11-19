#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <map>

class PantallaLCD;

class ConfigManager {
public:
    ConfigManager(PantallaLCD* lcd);

    void setModoConfiguracion(bool activo);
    bool getModoConfiguracion();

    // Procesar código IR recibido
    void procesarCodigo(uint32_t code);

private:
    bool modoConfiguracion = false;
    PantallaLCD* pantalla;

    const char* obtenerNombre(uint32_t code);
};

#endif
