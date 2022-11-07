#ifndef DEVICE_H
#define DEVICE_H

#include <M5EPD.h>
#include <WiFi.h>
#include <NTPClient.h>

#include "config.h"

class Device {
    public:
        Device(void) {};

        void init(void);
        GeneralConfiguration config(void);
        void saveConfig(void);

        bool startWiFi(void);
        void stopWiFi(void);
        bool isWiFiConnected(void);

        bool updateClock(void);

        String smartHomeApiBaseUrl();

    private:
        GeneralConfiguration _config;

        void mountFs(void);

        int _currentWiFi = 0;
        void logWiFiResult(uint8_t result);

        WiFiUDP _wifiUdp;
        NTPClient *_ntpClient;
};

extern Device EPD;

#endif