#include "IRManager.h"
#include <IRremote.h>

IRManager::IRManager(uint8_t recvPin)
    : pinRx(recvPin) {}

void IRManager::begin() {
    IrReceiver.begin(pinRx, ENABLE_LED_FEEDBACK);
}

bool IRManager::available() {
    return IrReceiver.decode();
}

uint32_t IRManager::getLastCode() {
    return IrReceiver.decodedIRData.decodedRawData;
}

void IRManager::resume() {
    IrReceiver.resume();
}
