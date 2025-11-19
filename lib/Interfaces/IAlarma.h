#ifndef IALARMA_H
#define IALARMA_H

#ifdef UNIT_TEST
    #include <string>

    // En modo test, usar std::string
    typedef std::string String;
#else
    #include <Arduino.h>
    // En modo real, usar String de Arduino
#endif

class IAlarma {
public:
    virtual ~IAlarma() = default;
    virtual void activar(const String& zona) = 0;
    virtual void desactivar() = 0;
    virtual void actualizar() = 0;
    virtual bool estaActiva() const = 0;
};

#endif