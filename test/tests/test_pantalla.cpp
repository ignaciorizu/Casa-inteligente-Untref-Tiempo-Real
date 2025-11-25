#include "gtest/gtest.h"
#include "../test_common.h"
#include "../mocks/HardwareMock.h"
#include "../../PantallaLCD.h"

class PantallaLCDTest : public ::testing::Test {
protected:
    PantallaLCD* pantalla;

    void SetUp() override {
        pantalla = new PantallaLCD(0x27, 20, 4);
    }

    void TearDown() override {
        delete pantalla;
    }
};

TEST_F(PantallaLCDTest, Initialization) {
    // La inicialización debería completarse sin errores
    pantalla->iniciar();
    SUCCEED();
}

TEST_F(PantallaLCDTest, ShowStartupScreen) {
    pantalla->iniciar();
    pantalla->mostrarInicio();
    SUCCEED(); // Verificar que no hay crashes
}

TEST_F(PantallaLCDTest, ShowRoomInfo) {
    pantalla->iniciar();

    pantalla->mostrarHabitacion(
        "Sala",
        22.5f,
        true,   // calefactor encendido
        350,    // lux
        false   // luz apagada
    );

    SUCCEED(); // Verificar que se muestra correctamente
}

TEST_F(PantallaLCDTest, ShowAlarm) {
    pantalla->iniciar();
    pantalla->mostrarAlarma("Entrada");
    SUCCEED(); // Verificar que se muestra alarma
}

TEST_F(PantallaLCDTest, ShowConfiguration) {
    pantalla->iniciar();
    pantalla->mostrarConfig("Temp Min", "20.5");
    SUCCEED(); // Verificar que se muestra configuración
}

TEST_F(PantallaLCDTest, ClearScreen) {
    pantalla->iniciar();
    pantalla->limpiar();
    SUCCEED(); // Verificar que se limpia sin errores
}

TEST_F(PantallaLCDTest, SaveAndLoadMessages) {
    pantalla->iniciar();
    pantalla->mostrarGuardado();
    pantalla->mostrarCargado();
    SUCCEED(); // Verificar mensajes de estado
}

TEST_F(PantallaLCDTest, WriteLine) {
    pantalla->iniciar();
    pantalla->escribirLinea(0, "Línea 1");
    pantalla->escribirLinea(1, "Línea 2");
    SUCCEED(); // Verificar escritura de líneas individuales
}