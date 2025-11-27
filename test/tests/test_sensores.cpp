#include "gtest/gtest.h"
#include "../test_common.h"
#include "../mocks/ArduinoMock.h"
#include "../mocks/HardwareMock.h"
#include "../../include/SensorTemperatura.h"
#include "../../include/SensorLuz.h"
#include "../../include/SensorMovimiento.h"

class SensorTest : public ::testing::Test {
protected:
    ConfigManager* config;

    void SetUp() override {
        TestHelper::resetArduinoMock();
        config = new ConfigManager();
    }

    void TearDown() override {
        delete config;
    }
};

// Tests para SensorTemperatura
TEST_F(SensorTest, SensorTemperaturaInitialization) {
    SensorTemperatura sensor(2, 27, *config);

    EXPECT_EQ(sensor.getLedPin(), 27);
    // Verificar que el pin del LED se configuró como OUTPUT
    EXPECT_EQ(pinModes[27], OUTPUT);
}

TEST_F(SensorTest, SensorTemperaturaRead) {
    SensorTemperatura sensor(2, 27, *config);

    // Simular lectura de temperatura
    sensor.actualizar();
    float temp = sensor.getTemperatura();

    // En el mock, debería devolver 22.0
    EXPECT_FALSE(std::isnan(temp));
    EXPECT_FLOAT_EQ(temp, 22.0f);
}

TEST_F(SensorTest, SensorTemperaturaActualizaLEDConTemperaturaBaja) {
    SensorTemperatura sensor(2, 27, *config);

    // Configurar temperatura baja (por debajo del mínimo)
    config->setTempMin(20.0f);
    config->setTempMax(25.0f);

    // Simular lectura de temperatura baja
    DHT* dhtMock = const_cast<DHT*>(&sensor.getDHTForTesting());
    dhtMock->setTemperature(18.0f); // Por debajo del mínimo

    sensor.actualizar();

    // Verificar que el LED se encendió (HIGH)
    EXPECT_EQ(pinStates[27], HIGH);
}

TEST_F(SensorTest, SensorTemperaturaActualizaLEDConTemperaturaAlta) {
    SensorTemperatura sensor(2, 27, *config);

    // Configurar temperatura
    config->setTempMin(20.0f);
    config->setTempMax(25.0f);

    // Simular lectura de temperatura alta
    DHT* dhtMock = const_cast<DHT*>(&sensor.getDHTForTesting());
    dhtMock->setTemperature(26.0f); // Por encima del máximo

    sensor.actualizar();

    // Verificar que el LED se apagó (LOW)
    EXPECT_EQ(pinStates[27], LOW);
}

// Tests para SensorLuz
TEST_F(SensorTest, SensorLuzInitialization) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Verificar que el pin del LED se configuró como OUTPUT
    EXPECT_EQ(pinModes[12], OUTPUT);
    // Verificar que el pin LDR se configuró correctamente (INPUT por defecto en analogRead)
}

TEST_F(SensorTest, SensorLuzLuxCalculation) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Configurar valor analógico simulado
    analogValues[32] = 2048; // Valor medio

    sensor.actualizar();
    float lux = sensor.getLux();

    EXPECT_FALSE(std::isnan(lux));
    EXPECT_GE(lux, 0.0f);

    // Verificar que el cálculo es razonable
    EXPECT_LE(lux, 10000.0f);
}

TEST_F(SensorTest, SensorLuzActualizaLEDConLuzBaja) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Configurar umbral
    config->setUmbralLux(100.0f);

    // Simular OSCURIDAD: valor ALTO en LDR = más resistencia = menos lux
    analogValues[32] = 4000; // Valor ALTO = oscuridad = lux BAJO

    sensor.actualizar();

    // Verificar que el LED se encendió (HIGH) porque está OSCURO (lux < umbral)
    EXPECT_EQ(pinStates[12], HIGH);
}

TEST_F(SensorTest, SensorLuzActualizaLEDConLuzAlta) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Configurar umbral
    config->setUmbralLux(100.0f);

    // Simular LUZ: valor BAJO en LDR = menos resistencia = más lux
    analogValues[32] = 100; // Valor BAJO = mucha luz = lux ALTO

    sensor.actualizar();

    // Verificar que el LED se apagó (LOW) porque hay LUZ (lux > umbral)
    EXPECT_EQ(pinStates[12], LOW);
}

TEST_F(SensorTest, SensorLuzSuavizadoExponencial) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Primer lectura
    analogValues[32] = 1000;
    sensor.actualizar();
    float primeraLectura = sensor.getLux();

    // Segunda lectura diferente
    analogValues[32] = 3000;
    sensor.actualizar();
    float segundaLectura = sensor.getLux();

    // Verificar que el suavizado funciona (segunda lectura no es igual a la primera)
    EXPECT_NE(primeraLectura, segundaLectura);
}

// Tests para SensorMovimiento
TEST_F(SensorTest, SensorMovimientoInitialization) {
    SensorMovimiento sensor(19);

    // Verificar que el pin se configuró como INPUT_PULLUP
    EXPECT_EQ(pinModes[19], INPUT_PULLUP);
}

TEST_F(SensorTest, SensorMovimientoDetection) {
    SensorMovimiento sensor(19);

    // Simular sin movimiento
    pinStates[19] = LOW;
    EXPECT_FALSE(sensor.hayMovimiento());

    // Simular con movimiento
    pinStates[19] = HIGH;
    EXPECT_TRUE(sensor.hayMovimiento());
}

TEST_F(SensorTest, SensorMovimientoCambioDeEstado) {
    SensorMovimiento sensor(19);

    // Probar transición de estados
    pinStates[19] = LOW;
    EXPECT_FALSE(sensor.hayMovimiento());

    pinStates[19] = HIGH;
    EXPECT_TRUE(sensor.hayMovimiento());

    pinStates[19] = LOW;
    EXPECT_FALSE(sensor.hayMovimiento());
}

// Tests de integración entre sensores y configuración
TEST_F(SensorTest, SensoresRespetanConfiguracion) {
    // Configurar valores específicos
    config->setTempMin(18.0f);
    config->setTempMax(26.0f);
    config->setUmbralLux(50.0f);

    SensorTemperatura sensorTemp(2, 27, *config);
    SensorLuz sensorLuz(32, 12, "Entrada", *config);

    // Verificar que los sensores pueden acceder a la configuración
    // sin crashes y que la configuración se aplica
    EXPECT_NO_THROW({
        sensorTemp.actualizar();
        sensorLuz.actualizar();
    });

    // Verificar que los sensores están vivos y funcionando
    EXPECT_FALSE(std::isnan(sensorTemp.getTemperatura()));
    EXPECT_FALSE(std::isnan(sensorLuz.getLux()));
}

// Tests de valores extremos
TEST_F(SensorTest, SensorLuzValoresExtremos) {
    SensorLuz sensor(32, 12, "Entrada", *config);

    // Test con valor mínimo
    analogValues[32] = 0;
    sensor.actualizar();
    EXPECT_FALSE(std::isnan(sensor.getLux()));

    // Test con valor máximo
    analogValues[32] = 4095;
    sensor.actualizar();
    EXPECT_FALSE(std::isnan(sensor.getLux()));
}

TEST_F(SensorTest, SensorTemperaturaValorInvalido) {
    SensorTemperatura sensor(2, 27, *config);

    // Simular lectura inválida (NAN)
    DHT* dhtMock = const_cast<DHT*>(&sensor.getDHTForTesting());
    dhtMock->setTemperature(NAN);

    sensor.actualizar();

    // El sensor debería manejar valores inválidos
    // sin crash y posiblemente mantener el último valor válido
    SUCCEED();
}

// Test de rendimiento - múltiples actualizaciones rápidas
TEST_F(SensorTest, MultiplesActualizacionesRapidas) {
    SensorMovimiento sensor(19);

    for(int i = 0; i < 100; i++) {
        pinStates[19] = (i % 2 == 0) ? HIGH : LOW;
        bool movimiento = sensor.hayMovimiento();
        EXPECT_EQ(movimiento, (i % 2 == 0));
    }
}

// Test de inicialización con pines inválidos
TEST_F(SensorTest, SensorMovimientoPinInvalido) {
    // Debería manejarse gracefulmente
    SensorMovimiento sensor(-1);
    EXPECT_NO_THROW(sensor.hayMovimiento());
}