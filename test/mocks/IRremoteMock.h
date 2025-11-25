#ifndef IRREMOTE_MOCK_H
#define IRREMOTE_MOCK_H

#include <cstdint>

// Estructura para mock de datos IR
struct IRReceiverMock {
    bool decoded = false;
    uint32_t decodedRawData = 0;
    bool available = false;

    // Métodos para controlar el estado del mock
    void setAvailable(bool avail) { available = avail; }
    void setRawData(uint32_t data) { decodedRawData = data; }
    void reset() {
        decoded = false;
        decodedRawData = 0;
        available = false;
    }
};

// Variable global para el mock
inline IRReceiverMock IrReceiverMock;

// Funciones mock para IRremote - implementadas inline
inline void IrReceiverBegin(uint8_t pin) {
    // Inicialización simulada - no hacer nada en mock
}

inline bool IrReceiverDecode() {
    return IrReceiverMock.available;
}

inline uint32_t IrReceiverGetDecodedRawData() {
    return IrReceiverMock.decodedRawData;
}

inline void IrReceiverResume() {
    IrReceiverMock.available = false;
    IrReceiverMock.decoded = false;
}

// ELIMINAR esta línea para evitar conflictos
// #define IrReceiver IrReceiverMock

#endif