#include "gtest/gtest.h"
#include "../test_common.h"
#include "../mocks/ArduinoMock.h"
#include "../../include/Alarma.h"

class AlarmaTest : public ::testing::Test {
protected:
    Alarma* alarma;

    void SetUp() override {
        /*
            #define LED_ALARM 25
            #define SPEAKER_PIN 26
        */
        TestHelper::resetArduinoMock();
        alarma = new Alarma(26, 25);
    }

    void TearDown() override {
        delete alarma;
    }
};

TEST_F(AlarmaTest, InitialState) {
    EXPECT_FALSE(alarma->estaActiva());
}

TEST_F(AlarmaTest, ActivateAlarm) {
    alarma->activar("Sala");
    EXPECT_TRUE(alarma->estaActiva());
}

TEST_F(AlarmaTest, DeactivateAlarm) {
    alarma->activar("Sala");
    EXPECT_TRUE(alarma->estaActiva());

    alarma->desactivar();
    EXPECT_FALSE(alarma->estaActiva());
}

TEST_F(AlarmaTest, MultipleActivation) {
    alarma->activar("Entrada");
    EXPECT_TRUE(alarma->estaActiva());

    // Reactivar no debería cambiar el estado
    alarma->activar("Pasillo");
    EXPECT_TRUE(alarma->estaActiva());
}

TEST_F(AlarmaTest, UpdateActivatesHardware) {
    // Estado inicial - verificar que está desactivada
    EXPECT_FALSE(alarma->estaActiva());
    EXPECT_EQ(pinStates[25], LOW);  // LED apagado
    EXPECT_EQ(pinStates[26], LOW);  // Speaker apagado

    // Activar la alarma
    alarma->activar("Sala");
    EXPECT_TRUE(alarma->estaActiva());

    // Llamar actualizar - debería activar el hardware
    alarma->actualizar();

    // Verificar que se activó el speaker y LED
    EXPECT_EQ(pinStates[25], HIGH);  // LED encendido (pin 25)
    EXPECT_EQ(pinStates[26], HIGH);  // Speaker activado (pin 26)

    // Verificar adicionalmente que tone() fue llamado con la frecuencia correcta
    EXPECT_GT(toneStates[26], 0);  // Debería tener una frecuencia configurada

    // Verificar que se configuraron los pines correctamente
    EXPECT_EQ(pinModes[25], OUTPUT);
    EXPECT_EQ(pinModes[26], OUTPUT);
}

TEST_F(AlarmaTest, UpdateWithoutActivation) {
    // Llamar actualizar sin activar primero - no debería hacer nada
    alarma->actualizar();

    // Verificar que el hardware permanece desactivado
    EXPECT_EQ(pinStates[25], LOW);   // LED apagado
    EXPECT_EQ(pinStates[26], LOW);   // Speaker apagado
    EXPECT_FALSE(alarma->estaActiva());
}

TEST_F(AlarmaTest, DeactivateTurnsOffHardware) {
    // Activar y verificar hardware encendido
    alarma->activar("Sala");
    alarma->actualizar();
    EXPECT_EQ(pinStates[25], HIGH);  // LED encendido
    EXPECT_EQ(pinStates[26], HIGH);  // Speaker activado

    // Desactivar y verificar hardware apagado
    alarma->desactivar();
    EXPECT_EQ(pinStates[25], LOW);   // LED apagado
    EXPECT_EQ(pinStates[26], LOW);   // Speaker apagado
    EXPECT_FALSE(alarma->estaActiva());

    // Verificar que noTone() limpió el estado
    EXPECT_EQ(toneStates.count(26), 0u);  // No debería haber frecuencia configurada
}

TEST_F(AlarmaTest, MultipleUpdates) {
    alarma->activar("Sala");

    // Primera actualización
    alarma->actualizar();
    EXPECT_EQ(pinStates[25], HIGH);
    EXPECT_EQ(pinStates[26], HIGH);
    int firstFrequency = toneStates[26];

    // Segunda actualización - debería mantener el estado
    alarma->actualizar();
    EXPECT_EQ(pinStates[25], HIGH);
    EXPECT_EQ(pinStates[26], HIGH);
    EXPECT_EQ(toneStates[26], firstFrequency);  // Misma frecuencia
}