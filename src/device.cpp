#include "device.h"

static const char *TAG = "EPD";

Device EPD = Device();

void Device::init() {
    ESP_LOGD(TAG, "EPD: init()");

    M5.begin(true, false, true, true, false);
    ESP_LOGD(TAG, "M5 started");

    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    M5.EPD.Clear(true);
    M5.TP.SetRotation(GT911::ROTATE_90);
    ESP_LOGD(TAG, "Display initialized");

    mountFs();
    ESP_LOGI(TAG, "Mounted LittleFS");

    loadConfiguration(_config);

    _ntpClient = new NTPClient(_wifiUdp, _config.NtpServer.c_str());
    auto tz = _config.Timezone.c_str();
    setenv("TZ", tz, 1);
    tzset();
    ESP_LOGI(TAG, "Timezone set to [%s]", tz);
}

void Device::saveConfig() {
    saveConfiguration(_config);
}

void Device::mountFs() {
    if (!LittleFS.begin()) {
        ESP_LOGE(TAG, "Failed to initialize LittleFS");
    }
}

bool Device::startWiFi(void) {
    auto network = _config.WiFi.SavedNetworks[_currentWiFi];

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(network.SSID.c_str(), network.Password.c_str());

    auto result = WiFi.waitForConnectResult();
    ESP_LOGD(TAG, "WiFi begin result: %d", result);
    logWiFiResult(result);

    return result == WL_CONNECTED;
}

void Device::stopWiFi(void) {
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
    delay(1000);
    ESP_LOGD(TAG, "WiFi stopped");
}

bool Device::isWiFiConnected() {
    return WiFi.isConnected();
}

bool Device::updateClock() {
    ESP_LOGD(TAG, "Updating device clock from NTP");
    if (!_ntpClient->update()) {
        return false;
    }

    long int epoch = _ntpClient->getEpochTime();
    timeval tv = {epoch, 0};
    settimeofday(&tv, NULL);
    ESP_LOGD(TAG, "Epoch %d", epoch);

    tm local;
    getLocalTime(&local);
    ESP_LOGD(TAG, "Local time %s", asctime(&local));

    rtc_time_t time(local.tm_hour, local.tm_min, local.tm_sec);
    M5.RTC.setTime(&time);

    rtc_date_t date(local.tm_wday, local.tm_mon, local.tm_mday, local.tm_year + 1900);
    M5.RTC.setDate(&date);
    return true;
}

void Device::logWiFiResult(uint8_t result) {
    switch (result) {
        case 0:
            ESP_LOGI(TAG, "WiFi: idle status");
            break;
        case 1:
            ESP_LOGI(TAG, "WiFi: No SSID available");
            break;
        case 2:
            ESP_LOGI(TAG, "WiFi: Scan completed");
            break;
        case 3:
            ESP_LOGI(TAG, "WiFi: Connected");
            break;
        case 4:
            ESP_LOGI(TAG, "WiFi: Connect failed");
            break;
        case 5:
            ESP_LOGI(TAG, "WiFi: Connection lost");
            break;
        case 6:
            ESP_LOGI(TAG, "WiFi: Disconnected");
            break;
    }
}

String Device::smartHomeApiBaseUrl() {
    return String(_config.SmartHome.BaseUrl);
}