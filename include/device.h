#ifndef DEVICE_H
#define DEVICE_H

#include <map>

#include "WiFi.h"
#include "config.h"

static const char *TAG = "EPD";

class Device {
    public:
        Device(void) {};

        void init(void);
        GeneralConfiguration config(void);
        void saveConfig(void);

        void startWiFi(void);
        void stopWiFi(void);

        String smartHomeApiBaseUrl();

    private:
        GeneralConfiguration _config;

        void mountFs(void);

        int _currentWiFi = 0;
        void logWiFiResult(uint8_t result);
};

extern Device EPD;

#endif