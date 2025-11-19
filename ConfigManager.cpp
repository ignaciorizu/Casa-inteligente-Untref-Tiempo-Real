#include "ConfigManager.h"
#include "PantallaLCD.h"

// Tabla de códigos → nombres
static std::map<uint32_t, const char*> tablaCodigos = {
    {0x5DA2FF00, "POWER     "},
    {0x1DE2FF00, "MENU      "},
    {0xDD22FF00, "TEST      "},
    {0xFD02FF00, "+         "},
    {0x3DC2FF00, "BACK      "},
    {0x1FE0FF00, "RETROCEDER"},
    {0x57A8FF00, "PLAY      "},
    {0x6F90FF00, "ADELANTAR "},
    {0x9768FF00, "0         "},
    {0x6798FF00, "-         "},
    {0x4FB0FF00, "C         "},
    {0xCF30FF00, "1         "},
    {0xE718FF00, "2         "},
    {0x857AFF00, "3         "},
    {0xEF10FF00, "4         "},
    {0xC738FF00, "5         "},
    {0xA55AFF00, "6         "},
    {0xBD42FF00, "7         "},
    {0xB54AFF00, "8         "},
    {0xAD52FF00, "9         "}
};


ConfigManager::ConfigManager(PantallaLCD* lcd)
    : pantalla(lcd) {}

void ConfigManager::setModoConfiguracion(bool activo) {
    modoConfiguracion = activo;

    pantalla->limpiar();

    if (activo) {
        pantalla->escribirLinea(0, "MODO CONFIG");
        pantalla->escribirLinea(1, "Esperando IR...");
    } else {
        pantalla->escribirLinea(0, "SALIR CONFIG");
        pantalla->escribirLinea(1, " ");
    }
}

bool ConfigManager::getModoConfiguracion() {
    return modoConfiguracion;
}


void ConfigManager::procesarCodigo(uint32_t code) {
    if (!modoConfiguracion) return;

    pantalla->limpiar();

    char linea1[20];
    sprintf(linea1, "CODIGO:%08lX", code);

    pantalla->escribirLinea(0, linea1);
    pantalla->escribirLinea(1, obtenerNombre(code));
}


const char* ConfigManager::obtenerNombre(uint32_t code) {
    if (tablaCodigos.count(code)) {
        return tablaCodigos[code];
    }
    return "DESCONOCIDO";
}
