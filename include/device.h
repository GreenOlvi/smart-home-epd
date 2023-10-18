#ifndef DEVICE_H
#define DEVICE_H

#include <M5EPD.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include <LittleFS.h>

#include "config.h"
#include "time_module.h"
#include "nvs_module.h"

class Device {
    public:
        Device(void) {};

        void init(void);
        GeneralConfiguration config(void);
        void saveConfig(void);

        bool startWiFi(void);
        void stopWiFi(void);
        bool isWiFiConnected(void);
        WiFiClient* getWiFiClient(void);
        char* getRootCA(void);

        String smartHomeApiBaseUrl();

        TimeModule Time = TimeModule(&M5.RTC);
        NvsModule Nvs;

    private:
        GeneralConfiguration _config;

        void mountFs(void);

        int _currentWiFi = 0;
        void logWiFiResult(uint8_t result);
        WiFiClient *_wifiClient = nullptr;
        char *_rootCA = nullptr;
};

extern Device EPD;

#endif