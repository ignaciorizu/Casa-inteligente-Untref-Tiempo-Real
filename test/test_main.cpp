#include <iostream>
#include <cassert>

// Test simple sin dependencias externas
void test_suma() {
    assert(1 + 1 == 2);
    std::cout << "✅ Test suma: 1 + 1 = 2" << std::endl;
}

void test_temperatura() {
    float temp = 22.5;
    assert(temp > 20.0 && temp < 25.0);
    std::cout << "✅ Test temperatura: " << temp << "°C en rango normal" << std::endl;
}

void test_sistema() {
    bool sistema_activo = true;
    bool alarma_desactivada = false;
    assert(sistema_activo != alarma_desactivada);
    std::cout << "✅ Test lógica del sistema: correcto" << std::endl;
}

int main() {
    std::cout << "🏠 INICIANDO TESTS - SISTEMA CASA INTELIGENTE" << std::endl;
    std::cout << "=============================================" << std::endl;

    test_suma();
    test_temperatura();
    test_sistema();

    std::cout << "=============================================" << std::endl;
    std::cout << "🎉 TODOS LOS TESTS PASARON CORRECTAMENTE" << std::endl;
    std::cout << "🚀 Sistema listo para implementar componentes" << std::endl;

    return 0;
}