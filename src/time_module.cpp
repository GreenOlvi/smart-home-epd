#include "time_module.h"

static const char *TAG = "TimeModule";

void TimeModule::init(const GeneralConfiguration *config)
{
    auto ntpServerLength = config->NtpServer.length() + 1;
    _ntpServer = new char[ntpServerLength];
    config->NtpServer.toCharArray(_ntpServer, ntpServerLength);

    auto timezoneLength = config->Timezone.length() + 1;
    _timezone = new char[timezoneLength];
    config->Timezone.toCharArray(_timezone, timezoneLength);

    setTimezone(_timezone);
}

bool TimeModule::trySetClockFromRtc()
{
    if (isSyncNeeded())
    {
        return false;
    }

    rtc_date_t date;
    M5.RTC.getDate(&date);
    rtc_time_t time;
    M5.RTC.getTime(&time);

    setClock(date.year, date.mon, date.day, time.hour, time.min, time.sec);

    tm local;
    getLocalTime(&local);
    char buf[20];
    toString(buf, local);
    ESP_LOGD(TAG, "Loaded clock [%s] from RTC", buf);

    return true;
}

void TimeModule::setTimezone(const char *tz)
{
    setenv("TZ", tz, 1);
    tzset();
    ESP_LOGD(TAG, "Timezone set to [%s]", tz);
}

void TimeModule::setClock(int16_t year, int8_t month, int8_t day, int8_t hour, int8_t minute, int8_t second)
{
    tm t = {0};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min = minute;
    t.tm_sec = second;

    time_t epoch = mktime(&t);
    setClock(epoch);
}

void TimeModule::setClock(const time_t epoch)
{
    setTimezone("UTC0");
    timeval tv = {epoch, 0};
    settimeofday(&tv, NULL);
    setTimezone(_timezone);
    ESP_LOGD(TAG, "Device time set to [%d]", epoch);
}

bool TimeModule::trySetClockFromNtp()
{
    configTzTime(_timezone, _ntpServer);
    ESP_LOGD(TAG, "Updating device clock from NTP [%s]", _ntpServer);

    time_t now = time(nullptr);
    while (now < 8 * 3600 * 2)
    {
        delay(250);
        now = time(nullptr);
    }

    tm local;
    getLocalTime(&local);
    ESP_LOGD(TAG, "Local time %s", asctime(&local));

    return true;
}

bool TimeModule::trySetClockAndRtcFromNtp()
{
    if (!trySetClockFromNtp())
    {
        return false;
    }

    setRtcFromClock();

    // save now to nvs last-ntp-synch
    return true;
}

void TimeModule::setRtcFromClock()
{
    time_t now = time(nullptr);
    tm dt;
    gmtime_r(&now, &dt);

    rtc_time_t time(dt.tm_hour, dt.tm_min, dt.tm_sec);
    rtc_date_t date(dt.tm_wday, dt.tm_mon + 1, dt.tm_mday, dt.tm_year + 1900);

    M5.RTC.setTime(&time);
    M5.RTC.setDate(&date);

    char buf[20];
    toString(buf, date, time);
    ESP_LOGD(TAG, "Saved datetime [%s] to RTC", buf);
}

bool TimeModule::isSyncNeeded()
{
    // read from nvs
    return true;
}

// buffer size 20
void TimeModule::toString(char *buffer, rtc_date_t &date, rtc_time_t &time)
{
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
        date.year, date.mon, date.day, time.hour, time.min, time.sec);
}

void TimeModule::toString(char *buffer, tm &tm)
{
    sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}