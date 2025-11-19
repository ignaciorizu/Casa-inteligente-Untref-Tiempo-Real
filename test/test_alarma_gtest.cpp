#include <gtest/gtest.h>
#include <iostream>
#include "../lib/Alarma/Alarma.h"

// Test Fixture para Alarma
class AlarmaTest : public ::testing::Test {
protected:
    Alarma* alarma;

    void SetUp() override {
        alarma = new Alarma(18, 19);  // pinSpeaker, pinLed
    }

    void TearDown() override {
        delete alarma;
    }
};

// Tests con Google Test
TEST_F(AlarmaTest, InicializacionCorrecta) {
    EXPECT_FALSE(alarma->estaActiva());
    std::cout << "✅ Alarma inicializada correctamente" << std::endl;
}

TEST_F(AlarmaTest, Activacion) {
    alarma->activar("Sala");
    EXPECT_TRUE(alarma->estaActiva());
    std::cout << "✅ Alarma activada correctamente" << std::endl;
}

TEST_F(AlarmaTest, Desactivacion) {
    alarma->activar("Cocina");
    alarma->desactivar();
    EXPECT_FALSE(alarma->estaActiva());
    std::cout << "✅ Alarma desactivada correctamente" << std::endl;
}

TEST_F(AlarmaTest, MultipleActivacion) {
    alarma->activar("Garaje");
    EXPECT_TRUE(alarma->estaActiva());

    alarma->activar("Sala");  // Cambiar zona
    EXPECT_TRUE(alarma->estaActiva());

    std::cout << "✅ Múltiples activaciones funcionando" << std::endl;
}

TEST_F(AlarmaTest, ActualizacionFunciona) {
    alarma->activar("Patio");

    // Probar que actualizar no cambia el estado activo
    for (int i = 0; i < 3; i++) {
        alarma->actualizar();
        EXPECT_TRUE(alarma->estaActiva());
    }
    std::cout << "✅ Actualización mantiene estado activo" << std::endl;
}

// Test sin fixture
TEST(AlarmaStandaloneTest, CreacionMultiple) {
    Alarma alarma1(18, 19);
    Alarma alarma2(20, 21);

    EXPECT_FALSE(alarma1.estaActiva());
    EXPECT_FALSE(alarma2.estaActiva());
    std::cout << "✅ Múltiples alarmas creadas correctamente" << std::endl;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}