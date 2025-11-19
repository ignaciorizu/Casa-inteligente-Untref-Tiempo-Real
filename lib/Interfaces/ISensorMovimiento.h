#ifndef ISENSOR_MOVIMIENTO_H
#define ISENSOR_MOVIMIENTO_H

class ISensorMovimiento {
public:
    virtual ~ISensorMovimiento() = default;
    virtual bool hayMovimiento() = 0;
};

#endif