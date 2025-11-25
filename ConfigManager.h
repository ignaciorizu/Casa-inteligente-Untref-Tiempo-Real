#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#ifdef UNIT_TEST
    #include "./test/mocks/ArduinoMock.h"
    #include "./test/mocks/PreferencesMock.h"

    #define PREFS Preferences::getInstance()
#else
    #include <Arduino.h>
    #include <Preferences.h>

    #define PREFS prefs
#endif

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
    int getTempMin() const { return tempMin; }
    int getTempMax() const { return tempMax; }

    void guardarEnMemoria();
    void cargarDesdeMemoria();

    // Setters para testing
    #ifdef UNIT_TEST
    void setTempMin(float temp) { tempMin = temp; }
    void setTempMax(float temp) { tempMax = temp; }
    void setUmbralLux(float lux) { umbralLux = lux; }
    #endif

private:
    bool modoConfiguracion = false;
    int opcionActual = 0; // 0=min temp, 1=max temp, 2=umbral luz

    float tempMin = 20.0;
    float tempMax = 24.0;
    float umbralLux = 100.0;

    #ifdef UNIT_TEST
    // En modo test, no necesitamos almacenar el objeto
    #else
    Preferences prefs;
    #endif
};

#endif
