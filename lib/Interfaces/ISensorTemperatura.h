#ifndef ISENSOR_TEMPERATURA_H
#define ISENSOR_TEMPERATURA_H

class ISensorTemperatura {
public:
    virtual ~ISensorTemperatura() = default;
    virtual void actualizar() = 0;
    virtual float getTemperatura() const = 0;
    virtual int getLedPin() const = 0;
};

#endif