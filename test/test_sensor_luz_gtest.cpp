#include <gtest/gtest.h>
#include <iostream>
#include <cstring>
#include "../lib/Sensores/SensorLuz.h"

// Test Fixture para SensorLuz
class SensorLuzTest : public ::testing::Test {
protected:
    SensorLuz* sensor;

    void SetUp() override {
        sensor = new SensorLuz(0, 13, "Sala");  // Usar números simples en lugar de A0, A1, etc.
    }

    void TearDown() override {
        delete sensor;
    }
};

// Tests con Google Test
TEST_F(SensorLuzTest, InicializacionCorrecta) {
    EXPECT_STREQ(sensor->getUbicacion(), "Sala");
    EXPECT_EQ(sensor->getLux(), 0.0f);  // Debería ser 0 al inicio
    std::cout << "✅ Sensor de luz inicializado correctamente" << std::endl;
}

TEST_F(SensorLuzTest, LecturaLuz) {
    int lectura = sensor->leerLuz();
    EXPECT_EQ(lectura, 1000);  // Valor mock fijo en modo test
    std::cout << "✅ Lectura de luz: " << lectura << std::endl;
}

TEST_F(SensorLuzTest, ActualizacionLux) {
    sensor->actualizar();
    float lux = sensor->getLux();

    // En modo test: lectura (1000) * 0.1 = 100.0
    EXPECT_FLOAT_EQ(lux, 100.0f);
    std::cout << "✅ Lux actualizado: " << lux << " lx" << std::endl;
}

TEST_F(SensorLuzTest, MultiplesActualizaciones) {
    for (int i = 0; i < 3; i++) {
        sensor->actualizar();
        float lux = sensor->getLux();

        EXPECT_FLOAT_EQ(lux, 100.0f);  // Siempre el mismo valor en modo test
        std::cout << "📊 Actualización " << (i + 1) << ": " << lux << " lx" << std::endl;
    }
    std::cout << "✅ Múltiples actualizaciones funcionando" << std::endl;
}

TEST_F(SensorLuzTest, ControlLuz) {
    // En modo test, estos métodos no deberían causar errores
    sensor->encenderLuz();
    sensor->apagarLuz();

    std::cout << "✅ Control de luz funcionando" << std::endl;
}

TEST_F(SensorLuzTest, UbicacionConsistente) {
    const char* ubicacion1 = sensor->getUbicacion();
    const char* ubicacion2 = sensor->getUbicacion();

    EXPECT_STREQ(ubicacion1, "Sala");
    EXPECT_STREQ(ubicacion2, "Sala");
    EXPECT_EQ(ubicacion1, ubicacion2);  // Misma dirección de memoria

    std::cout << "✅ Ubicación consistente: " << ubicacion1 << std::endl;
}

// Test sin fixture
TEST(SensorLuzStandaloneTest, CreacionMultiple) {
    SensorLuz sensor1(1, 12, "Cocina");    // Usar números simples
    SensorLuz sensor2(2, 13, "Dormitorio");
    SensorLuz sensor3(3, 14, "Baño");

    EXPECT_STREQ(sensor1.getUbicacion(), "Cocina");
    EXPECT_STREQ(sensor2.getUbicacion(), "Dormitorio");
    EXPECT_STREQ(sensor3.getUbicacion(), "Baño");

    // Verificar lecturas
    EXPECT_EQ(sensor1.leerLuz(), 1000);
    EXPECT_EQ(sensor2.leerLuz(), 1000);
    EXPECT_EQ(sensor3.leerLuz(), 1000);

    std::cout << "✅ Múltiples sensores creados correctamente" << std::endl;
}

TEST(SensorLuzStandaloneTest, DiferentesUbicaciones) {
    SensorLuz sensorExterior(4, 15, "Exterior");
    SensorLuz sensorGaraje(5, 16, "Garaje");

    EXPECT_STREQ(sensorExterior.getUbicacion(), "Exterior");
    EXPECT_STREQ(sensorGaraje.getUbicacion(), "Garaje");

    sensorExterior.actualizar();
    sensorGaraje.actualizar();

    EXPECT_FLOAT_EQ(sensorExterior.getLux(), 100.0f);
    EXPECT_FLOAT_EQ(sensorGaraje.getLux(), 100.0f);

    std::cout << "✅ Sensores en diferentes ubicaciones funcionando" << std::endl;
}

TEST(SensorLuzStandaloneTest, ComportamientoInicial) {
    SensorLuz sensor(6, 17, "Oficina");

    // Antes de actualizar, lux debería ser 0
    EXPECT_FLOAT_EQ(sensor.getLux(), 0.0f);

    // Después de actualizar, debería tener el valor calculado
    sensor.actualizar();
    EXPECT_FLOAT_EQ(sensor.getLux(), 100.0f);

    std::cout << "✅ Comportamiento inicial correcto" << std::endl;
}

TEST(SensorLuzStandaloneTest, SecuenciaCompleta) {
    SensorLuz sensor(7, 18, "Jardín");

    // Secuencia típica de uso
    sensor.actualizar();
    float lux1 = sensor.getLux();

    int lectura = sensor.leerLuz();
    sensor.actualizar();
    float lux2 = sensor.getLux();

    sensor.encenderLuz();
    sensor.apagarLuz();

    EXPECT_EQ(lectura, 1000);
    EXPECT_FLOAT_EQ(lux1, 100.0f);
    EXPECT_FLOAT_EQ(lux2, 100.0f);

    std::cout << "✅ Secuencia completa ejecutada correctamente" << std::endl;
}

// Test de integración
TEST(SensorLuzIntegrationTest, SistemaIluminacion) {
    SensorLuz sensorPrincipal(8, 12, "Sala Principal");
    SensorLuz sensorSecundario(9, 13, "Pasillo");

    // Simular ciclo de monitoreo
    for (int i = 0; i < 3; i++) {
        sensorPrincipal.actualizar();
        sensorSecundario.actualizar();

        float luxPrincipal = sensorPrincipal.getLux();
        float luxSecundario = sensorSecundario.getLux();

        EXPECT_FLOAT_EQ(luxPrincipal, 100.0f);
        EXPECT_FLOAT_EQ(luxSecundario, 100.0f);

        std::cout << "🏠 Ciclo " << (i + 1)
                  << " - " << sensorPrincipal.getUbicacion() << ": " << luxPrincipal << " lx"
                  << ", " << sensorSecundario.getUbicacion() << ": " << luxSecundario << " lx" << std::endl;
    }
    std::cout << "✅ Sistema de iluminación funcionando correctamente" << std::endl;
}

// Test adicional para verificar que los números de pin funcionan correctamente
TEST(SensorLuzPinTest, PinesDigitales) {
    // Probar con pines digitales regulares también
    SensorLuz sensorDigital(10, 3, "Entrada");

    sensorDigital.actualizar();
    EXPECT_FLOAT_EQ(sensorDigital.getLux(), 100.0f);
    EXPECT_EQ(sensorDigital.leerLuz(), 1000);

    std::cout << "✅ Sensores en pines digitales funcionando" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}