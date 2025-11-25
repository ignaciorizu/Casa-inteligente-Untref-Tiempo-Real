#ifndef SENSOR_MOVIMIENTO_H
#define SENSOR_MOVIMIENTO_H

#ifdef UNIT_TEST
    #include "./test/mocks/ArduinoMock.h"
#else
    #include <Arduino.h>
#endif

class SensorMovimiento {
private:
    int pin;

public:
    SensorMovimiento(int p);

    bool hayMovimiento();
};

#endif
