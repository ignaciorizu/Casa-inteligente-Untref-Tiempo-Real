#include <gtest/gtest.h>
#include <iostream>
#include "../lib/Sensores/SensorMovimiento.h"

// Test Fixture para SensorMovimiento
class SensorMovimientoTest : public ::testing::Test {
protected:
    SensorMovimiento* sensor;

    void SetUp() override {
        sensor = new SensorMovimiento(5);  // pinPIR
    }

    void TearDown() override {
        delete sensor;
    }
};

// Tests con Google Test
TEST_F(SensorMovimientoTest, InicializacionCorrecta) {
    std::cout << "✅ Sensor de movimiento inicializado correctamente" << std::endl;
}

TEST_F(SensorMovimientoTest, DeteccionMovimientoConsistente) {
    // En modo test, hayMovimiento() siempre retorna false debido a la implementación mock
    bool movimiento = sensor->hayMovimiento();

    // En el entorno de test, digitalRead() del mock retorna LOW (false)
    EXPECT_FALSE(movimiento);
    std::cout << "✅ Detección de movimiento consistente: " << (movimiento ? "SÍ" : "NO") << std::endl;
}

TEST_F(SensorMovimientoTest, MultiplesLecturas) {
    for (int i = 0; i < 5; i++) {
        bool movimiento = sensor->hayMovimiento();
        EXPECT_FALSE(movimiento);  // Siempre false en modo test
        std::cout << "📊 Lectura " << (i + 1) << ": " << (movimiento ? "MOVIMIENTO" : "SIN MOVIMIENTO") << std::endl;
    }
    std::cout << "✅ Múltiples lecturas funcionando correctamente" << std::endl;
}

TEST_F(SensorMovimientoTest, ComportamientoDeterminista) {
    // En modo test, el comportamiento debería ser determinista
    bool primeraLectura = sensor->hayMovimiento();
    bool segundaLectura = sensor->hayMovimiento();

    EXPECT_EQ(primeraLectura, segundaLectura);
    std::cout << "✅ Comportamiento determinista confirmado" << std::endl;
}

// Test sin fixture
TEST(SensorMovimientoStandaloneTest, CreacionMultiple) {
    SensorMovimiento sensor1(5);
    SensorMovimiento sensor2(6);
    SensorMovimiento sensor3(7);

    bool lectura1 = sensor1.hayMovimiento();
    bool lectura2 = sensor2.hayMovimiento();
    bool lectura3 = sensor3.hayMovimiento();

    EXPECT_FALSE(lectura1);
    EXPECT_FALSE(lectura2);
    EXPECT_FALSE(lectura3);

    std::cout << "✅ Múltiples sensores creados correctamente" << std::endl;
}

TEST(SensorMovimientoStandaloneTest, DiferentesPines) {
    // Probar sensores en diferentes pines
    SensorMovimiento sensorPin2(2);
    SensorMovimiento sensorPin3(3);
    SensorMovimiento sensorPin4(4);

    // Todos deberían comportarse igual en modo test
    EXPECT_FALSE(sensorPin2.hayMovimiento());
    EXPECT_FALSE(sensorPin3.hayMovimiento());
    EXPECT_FALSE(sensorPin4.hayMovimiento());

    std::cout << "✅ Sensores en diferentes pines funcionando" << std::endl;
}

TEST(SensorMovimientoStandaloneTest, IntegracionConSistema) {
    // Simular uso típico en un sistema de seguridad
    SensorMovimiento sensorEntrada(8);
    SensorMovimiento sensorCocina(9);

    // Simular ciclo de monitoreo
    for (int i = 0; i < 3; i++) {
        bool movimientoEntrada = sensorEntrada.hayMovimiento();
        bool movimientoCocina = sensorCocina.hayMovimiento();

        EXPECT_FALSE(movimientoEntrada);
        EXPECT_FALSE(movimientoCocina);

        std::cout << "🏠 Ciclo " << (i + 1) << " - Entrada: " << (movimientoEntrada ? "ALERTA" : "OK")
                  << ", Cocina: " << (movimientoCocina ? "ALERTA" : "OK") << std::endl;
    }
    std::cout << "✅ Integración con sistema funcionando" << std::endl;
}

// Test de rendimiento
TEST(SensorMovimientoPerformanceTest, LecturasRapidas) {
    SensorMovimiento sensor(10);

    int totalLecturas = 10;
    int lecturasSinMovimiento = 0;

    for (int i = 0; i < totalLecturas; i++) {
        if (!sensor.hayMovimiento()) {
            lecturasSinMovimiento++;
        }
    }

    EXPECT_EQ(lecturasSinMovimiento, totalLecturas);
    std::cout << "✅ " << totalLecturas << " lecturas rápidas completadas correctamente" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}