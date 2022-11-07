#include "time_module.h"

static const char *TAG = "TimeModule";

void TimeModule::init(const GeneralConfiguration *config) {
    _ntpClient = new NTPClient(_wifiUdp, config->NtpServer.c_str());

    setTimezone(config->Timezone.c_str());

    rtc_date_t date;
    M5.RTC.getDate(&date);
    rtc_time_t time;
    M5.RTC.getTime(&time);

    setTime(date.year, date.mon, date.day, time.hour, time.min, time.sec);
}

void TimeModule::setTimezone(const char *tz) {
    setenv("TZ", tz, 1);
    tzset();
    ESP_LOGI(TAG, "Timezone set to [%s]", tz);
}

void TimeModule::setTime(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t minute, int8_t second) {
    tm t = {0};
    t.tm_year = year - 1900;
    t.tm_mon = month;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    time_t epoch = mktime(&t);
    setTime(epoch);
}

void TimeModule::setTime(const time_t epoch) {
    timeval tv = {epoch, 0};
    settimeofday(&tv, NULL);
    ESP_LOGD(TAG, "Device time set to [%d]", epoch);
}

bool TimeModule::updateClock() {
    ESP_LOGD(TAG, "Updating device clock from NTP");
    if (!_ntpClient->update()) {
        return false;
    }

    long int epoch = _ntpClient->getEpochTime();
    setTime(epoch);

    tm local;
    getLocalTime(&local);
    ESP_LOGD(TAG, "Local time %s", asctime(&local));

    rtc_time_t time(local.tm_hour, local.tm_min, local.tm_sec);
    M5.RTC.setTime(&time);

    rtc_date_t date(local.tm_wday, local.tm_mon, local.tm_mday, local.tm_year + 1900);
    M5.RTC.setDate(&date);
    return true;
}