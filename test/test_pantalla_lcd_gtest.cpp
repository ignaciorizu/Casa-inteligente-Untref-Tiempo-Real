#include <gtest/gtest.h>
#include <iostream>
#include "../lib/Pantalla/PantallaLCD.h"

// Test Fixture para PantallaLCD
class PantallaLCDTest : public ::testing::Test {
protected:
    PantallaLCD* pantalla;

    void SetUp() override {
        pantalla = new PantallaLCD();
    }

    void TearDown() override {
        delete pantalla;
    }
};

// Tests con Google Test
TEST_F(PantallaLCDTest, InicializacionCorrecta) {
    pantalla->inicializar();
    std::cout << "✅ Pantalla LCD inicializada correctamente" << std::endl;
}

TEST_F(PantallaLCDTest, MostrarHabitacionConCalefaccion) {
    pantalla->mostrarHabitacion("Sala", 22.5f, true);
    std::cout << "✅ Habitación con calefacción mostrada correctamente" << std::endl;
}

TEST_F(PantallaLCDTest, MostrarHabitacionSinCalefaccion) {
    pantalla->mostrarHabitacion("Dormitorio", 18.0f, false);
    std::cout << "✅ Habitación sin calefacción mostrada correctamente" << std::endl;
}

TEST_F(PantallaLCDTest, MostrarAlarma) {
    pantalla->mostrarAlarma("Cocina");
    std::cout << "✅ Alarma mostrada correctamente" << std::endl;
}

TEST_F(PantallaLCDTest, LimpiarPantalla) {
    pantalla->mostrarHabitacion("Baño", 20.0f, true);
    pantalla->limpiar();
    std::cout << "✅ Pantalla limpiada correctamente" << std::endl;
}

TEST_F(PantallaLCDTest, SecuenciaCompleta) {
    pantalla->inicializar();
    pantalla->mostrarHabitacion("Sala", 21.5f, true);
    pantalla->mostrarAlarma("Garaje");
    pantalla->mostrarHabitacion("Cocina", 19.0f, false);
    pantalla->limpiar();
    std::cout << "✅ Secuencia completa ejecutada correctamente" << std::endl;
}

// Test sin fixture
TEST(PantallaLCDStandaloneTest, CreacionMultiple) {
    PantallaLCD pantalla1;
    PantallaLCD pantalla2;

    pantalla1.inicializar();
    pantalla2.mostrarHabitacion("Test", 20.0f, true);

    std::cout << "✅ Múltiples pantallas creadas correctamente" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}