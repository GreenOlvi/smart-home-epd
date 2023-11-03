#ifndef TIME_MODULE_H
#define TIME_MODULE_H

#include <M5EPD.h>
#include <WiFi.h>
#include <NTPClient.h>

#include "nvs_module.h"
#include "config.h"

#define RtcSyncIntervalInSeconds (60*60*24*7) // once a week

class TimeModule {
    public:
        TimeModule(const BM8563 *rtcModule, const NvsModule *nvsModule)
            : _rtcModule(rtcModule), _nvs(nvsModule)
        {};

        void init(const GeneralConfiguration *config);
        bool trySetClockFromRtc(void);
        bool trySetClockFromNtp(void);
        bool trySetClockAndRtcFromNtp(void);

    private:
        const BM8563 *_rtcModule;
        const NvsModule *_nvs;
        char *_ntpServer;
        char *_timezone;

        void setTimezone(const char *tz);
        void setClock(const int16_t year, const int8_t month, const int8_t day,
            const int8_t hour, const int8_t minute, const int8_t second);
        void setClock(const time_t epoch);
        void setRtcFromClock(void);
        bool isSyncNeeded(void);
        void toString(char *buffer, rtc_date_t &date, rtc_time_t &time);
        void toString(char *buffer, tm &tm);
};

#endif