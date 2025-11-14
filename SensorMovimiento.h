#ifndef SENSOR_MOVIMIENTO_H
#define SENSOR_MOVIMIENTO_H

#include <Arduino.h>

class SensorMovimiento {
private:
    int pin;

public:
    SensorMovimiento(int p);

    bool hayMovimiento();
};

#endif
