#ifndef TIME_MODULE_H
#define TIME_MODULE_H

#include <M5EPD.h>
#include <WiFi.h>
#include <NTPClient.h>

#include "config.h"

class TimeModule {
    public:
        TimeModule(const BM8563 *rtcModule) : _rtcModule(rtcModule) {};

        void init(const GeneralConfiguration *config);
        bool updateClock(void);

    private:
        const BM8563 *_rtcModule;
        WiFiUDP _wifiUdp;
        NTPClient *_ntpClient;

        void setTimezone(const char *tz);
        void setTime(const int16_t year, const int8_t month, const int8_t day,
            const int8_t hour, const int8_t minute, const int8_t second);
        void setTime(const time_t epoch);
};

#endif