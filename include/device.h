#ifndef DEVICE_H
#define DEVICE_H

#include <M5EPD.h>
#include <WiFi.h>

#include "config.h"
#include "time_module.h"

class Device {
    public:
        Device(void) {};

        void init(void);
        GeneralConfiguration config(void);
        void saveConfig(void);

        bool startWiFi(void);
        void stopWiFi(void);
        bool isWiFiConnected(void);

        String smartHomeApiBaseUrl();

        TimeModule Time = TimeModule(&M5.RTC);

    private:
        GeneralConfiguration _config;

        void mountFs(void);

        int _currentWiFi = 0;
        void logWiFiResult(uint8_t result);
};

extern Device EPD;

#endif