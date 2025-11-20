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
    String getTextoOpcion() const;
    String getValorActual() const;

    void setModoConfig(bool activo);

    void incrementarValor();
    void decrementarValor();
    
    int getUmbralLux() const { return umbralLux; }

private:
    bool modoConfiguracion = false;
    int opcionActual = 0; // 0=min temp, 1=max temp, 2=umbral luz

    float tempMin = 20.0;
    float tempMax = 24.0;
    float umbralLux = 100.0;
};

#endif
