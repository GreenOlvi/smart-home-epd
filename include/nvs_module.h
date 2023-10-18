#ifndef NVS_MODULE_H
#define NVS_MODULE_H

#include <Arduino.h>
#include <nvs_flash.h>
#include <nvs.h>

class NvsModule
{
    public:
        NvsModule(void) {};

        void init(void);

        void setValueU8(const char* key, const uint8_t value);
        bool tryGetValueU8(const char* key, uint8_t* value);
        uint8_t getValueU8(const char* key, const uint8_t defaultValue = 0);

    private:

};

#endif