#include "ConfigManager.h"

ConfigManager::ConfigManager() {}

void ConfigManager::setModoConfig(bool activo) {
    modoConfiguracion = activo;
    opcionActual = 0;
}

void ConfigManager::procesarCodigoIR(IRButton boton) {
    if (!modoConfiguracion) {
        return;
    }

    switch (boton) {

        case IR_ADELANTAR:
            opcionActual++;
            if (opcionActual > 2) {
                opcionActual = 0;
            }
            break;

        case IR_RETROCEDER:
            if (opcionActual == 0) {
                opcionActual = 2;
            } else {
                opcionActual--;
            }
            break;

        case IR_MAS:
            incrementarValor();
            break;

        case IR_MENOS:
            decrementarValor();
            break;

        default:
            break;
    }
}

void ConfigManager::incrementarValor() {
    switch (opcionActual) {

        case 0:  // Temp Min
            if (tempMin < 45.0) {
                tempMin += 0.5;
            }
            break;

        case 1:  // Temp Max
            if (tempMax < 45.0) {
                tempMax += 0.5;
            }
            break;

        case 2:  // Umbral de Luz
            if (umbralLux < 150) {
                umbralLux += 10;
            } else if (umbralLux < 2000) {
                umbralLux += 50;
            } else if (umbralLux < 10000) {
                umbralLux += 100;
            }
            break;
    }
}

void ConfigManager::decrementarValor() {
    switch (opcionActual) {

        case 0:  // Temp Min
            if (tempMin > 0.0) {
                tempMin -= 0.5;
            }
            break;

        case 1:  // Temp Max
            if (tempMax > 0.0) {
                tempMax -= 0.5;
            }
            break;

        case 2:  // Umbral de Luz
            if (umbralLux > 2000) {
                umbralLux -= 100;
            } else if (umbralLux > 150) {
                umbralLux -= 50;
            } else if (umbralLux > 0) {
                umbralLux -= 10;
            }
            break;
    }
}

String ConfigManager::getTextoOpcion() const {
    switch (opcionActual) {
        case 0: return "Temp Min (C)";
        case 1: return "Temp Max (C)";
        case 2: return "Umbral Luz";
        default: return "";
    }
}

String ConfigManager::getValorActual() const {
    switch (opcionActual) {
        case 0: return String(tempMin, 1);
        case 1: return String(tempMax, 1);
        case 2: return String(umbralLux);
        default: return "";
    }
}


void ConfigManager::guardarEnMemoria() {
    PREFS.begin("config", false);
    PREFS.putFloat("tMin", tempMin);
    PREFS.putFloat("tMax", tempMax);
    PREFS.putUInt("lux", (unsigned int)umbralLux);
    PREFS.end();
}

void ConfigManager::cargarDesdeMemoria() {
    PREFS.begin("config", true);

    float vMin = PREFS.getFloat("tMin", tempMin);
    float vMax = PREFS.getFloat("tMax", tempMax);
    unsigned int vLux = PREFS.getUInt("lux", (unsigned int)umbralLux);

    PREFS.end();

    // Validaciones
    if (vMin > 0 && vMin < 50) tempMin = vMin;
    if (vMax > 0 && vMax < 50) tempMax = vMax;
    if (vLux > 0 && vLux < 20000) umbralLux = vLux;
}
