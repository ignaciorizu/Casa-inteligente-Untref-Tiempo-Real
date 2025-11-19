#ifndef SENSOR_MOVIMIENTO_H
#define SENSOR_MOVIMIENTO_H

#include "../Interfaces/ISensorMovimiento.h"

class SensorMovimiento : public ISensorMovimiento {
private:
    int pin;

public:
    SensorMovimiento(int p);
    bool hayMovimiento() override;
};

#endif