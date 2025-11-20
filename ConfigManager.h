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

    void procesarCodigoIR(uint32_t code);
    
    const char* obtenerNombre(uint32_t code);

private:
    bool modoConfiguracion = false;
    PantallaLCD* pantalla;
};

#endif
