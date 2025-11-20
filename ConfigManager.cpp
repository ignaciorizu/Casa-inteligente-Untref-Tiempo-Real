#include "ConfigManager.h"

ConfigManager::ConfigManager() {}

void ConfigManager::setModoConfig(bool activo) {
    modoConfiguracion = activo;
    opcionActual = 0;
}

void ConfigManager::procesarCodigoIR(IRButton boton) {
    if (!modoConfiguracion) return;

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
        case 0: 
            if (tempMin < 45.0) {
                tempMin += 0.5;
            } 
            break;
        case 1: 
            if (tempMax < 45.0) {
                tempMax += 0.5;
            } 
            break;
        case 2: 
            if (umbralLux < 9950) {
                umbralLux += 50;
            }
            break;
    }
}

void ConfigManager::decrementarValor() {
    switch (opcionActual) {
        case 0: 
            if (tempMin > 0.0) {
                tempMin -= 0.5;
            }
            break;
        case 1: 
            if (tempMax > 0.0) {
                tempMax -= 0.5;
            }
            break;
        case 2: 
            if (umbralLux > 1) {
                umbralLux -= 50;
            }
            break;
    }
}

String ConfigManager::getTextoOpcion() const {
    switch (opcionActual) {
        case 0: return "Temp Min (°C)";
        case 1: return "Temp Max (°C)";
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
