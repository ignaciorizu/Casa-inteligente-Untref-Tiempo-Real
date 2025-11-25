#ifndef PREFERENCES_MOCK_H
#define PREFERENCES_MOCK_H

#include <map>
#include <string>
#include <memory>

class Preferences {
private:
    std::string namespace_;
    std::map<std::string, float> floatValues;
    std::map<std::string, int> intValues;
    std::map<std::string, unsigned int> uintValues;
    bool begun;

    // Hacer el constructor privado para singleton
    Preferences() : begun(false) {}

public:
    // Eliminar copia
    Preferences(const Preferences&) = delete;
    Preferences& operator=(const Preferences&) = delete;

    // Método singleton
    static Preferences& getInstance() {
        static Preferences instance;
        return instance;
    }

    bool begin(const char* name, bool readOnly) {
        namespace_ = name;
        begun = true;
        return true;
    }

    void end() {
        begun = false;
    }

    void putFloat(const char* key, float value) {
        if(begun) floatValues[key] = value;
    }

    float getFloat(const char* key, float defaultValue) {
        if(begun && floatValues.count(key)) return floatValues[key];
        return defaultValue;
    }

    void putUInt(const char* key, unsigned int value) {
        if(begun) uintValues[key] = value;
    }

    unsigned int getUInt(const char* key, unsigned int defaultValue) {
        if(begun && uintValues.count(key)) return uintValues[key];
        return defaultValue;
    }

    // Para testing - limpiar entre tests
    void clear() {
        floatValues.clear();
        intValues.clear();
        uintValues.clear();
        begun = false;
    }

    bool hasKey(const char* key) const {
        return floatValues.count(key) || intValues.count(key) || uintValues.count(key);
    }
};

#endif