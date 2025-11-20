#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <map>
#include "IRCodes.h"

class ConfigManager {
public:
    ConfigManager();

    void procesarCodigoIR(IRButton boton);

    bool enMenu() const { return modoConfiguracion; }
    int getOpcion() const { return opcionActual; }
    String getTextoOpcion() const;
    String getValorActual() const;

    // Activar/Desactivar menú
    void setModoConfig(bool activo);

private:
    void incrementarValor();
    void decrementarValor();

    bool modoConfiguracion = false;
    int opcionActual = 0; // 0=min temp, 1=max temp, 2=umbral luz

    // Valores internos (simulados)
    float tempMin = 20.0;
    float tempMax = 24.0;
    float umbralLux = 300.0;
};

#endif
