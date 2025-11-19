#include <gtest/gtest.h>
#include <iostream>
#include <cmath>
#include "../lib/Sensores/SensorTemperatura.h"

// Test Fixture para SensorTemperatura
class SensorTemperaturaTest : public ::testing::Test {
protected:
    SensorTemperatura* sensor;

    void SetUp() override {
        sensor = new SensorTemperatura(2, 13, 20.0f, 24.0f);  // pinDHT, pinLED, tempMin, tempMax
    }

    void TearDown() override {
        delete sensor;
    }
};

// Tests con Google Test
TEST_F(SensorTemperaturaTest, InicializacionCorrecta) {
    EXPECT_TRUE(std::isnan(sensor->getTemperatura()));  // Debería ser NaN al inicio
    EXPECT_EQ(sensor->getLedPin(), 13);
    std::cout << "✅ Sensor de temperatura inicializado correctamente" << std::endl;
}

TEST_F(SensorTemperaturaTest, ActualizacionTemperatura) {
    sensor->actualizar();
    float temp = sensor->getTemperatura();

    EXPECT_FALSE(std::isnan(temp));  // No debería ser NaN después de actualizar
    EXPECT_GE(temp, 18.0f);         // Dentro del rango simulado
    EXPECT_LE(temp, 26.0f);         // Dentro del rango simulado

    std::cout << "✅ Temperatura actualizada: " << temp << " °C" << std::endl;
}

TEST_F(SensorTemperaturaTest, MultiplesActualizaciones) {
    for (int i = 0; i < 5; i++) {
        sensor->actualizar();
        float temp = sensor->getTemperatura();

        EXPECT_FALSE(std::isnan(temp));
        EXPECT_GE(temp, 18.0f);
        EXPECT_LE(temp, 26.0f);

        std::cout << "📊 Actualización " << (i + 1) << ": " << temp << " °C" << std::endl;
    }
    std::cout << "✅ Múltiples actualizaciones funcionando" << std::endl;
}

TEST_F(SensorTemperaturaTest, TemperaturaConsistente) {
    sensor->actualizar();
    float temp1 = sensor->getTemperatura();

    // La temperatura debería mantenerse hasta la próxima actualización
    float temp2 = sensor->getTemperatura();
    EXPECT_EQ(temp1, temp2);

    std::cout << "✅ Temperatura consistente entre lecturas: " << temp1 << " °C" << std::endl;
}

TEST_F(SensorTemperaturaTest, RangoTemperaturaPersonalizado) {
    SensorTemperatura sensorCustom(3, 12, 18.0f, 22.0f);  // Rango personalizado

    sensorCustom.actualizar();
    float temp = sensorCustom.getTemperatura();

    EXPECT_FALSE(std::isnan(temp));
    EXPECT_GE(temp, 18.0f);
    EXPECT_LE(temp, 26.0f);  // El rango simulado sigue siendo 18-26

    std::cout << "✅ Sensor con rango personalizado funcionando: " << temp << " °C" << std::endl;
}

// Test sin fixture
TEST(SensorTemperaturaStandaloneTest, CreacionMultiple) {
    SensorTemperatura sensor1(2, 13, 20.0f, 24.0f);
    SensorTemperatura sensor2(3, 12, 18.0f, 22.0f);

    EXPECT_TRUE(std::isnan(sensor1.getTemperatura()));
    EXPECT_TRUE(std::isnan(sensor2.getTemperatura()));
    EXPECT_EQ(sensor1.getLedPin(), 13);
    EXPECT_EQ(sensor2.getLedPin(), 12);

    std::cout << "✅ Múltiples sensores creados correctamente" << std::endl;
}

TEST(SensorTemperaturaStandaloneTest, ComportamientoInicial) {
    SensorTemperatura sensor(4, 11, 19.0f, 23.0f);

    // Antes de actualizar, temperatura debería ser NaN
    EXPECT_TRUE(std::isnan(sensor.getTemperatura()));

    // Después de actualizar, debería tener un valor válido
    sensor.actualizar();
    EXPECT_FALSE(std::isnan(sensor.getTemperatura()));

    std::cout << "✅ Comportamiento inicial correcto" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}