#ifndef IRMANAGER_H
#define IRMANAGER_H

#include <Arduino.h>

class IRManager {
public:
    IRManager(uint8_t recvPin);

    void begin();
    bool available();
    uint32_t getLastCode();
    void resume();

private:
    uint8_t pinRx;
};

#endif
