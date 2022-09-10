#include "Device.h"

Device EPD = Device();

void Device::init() {
    mountFs();
    loadConfiguration(_config);
}

void Device::saveConfig() {
    saveConfiguration(_config);
}

void Device::mountFs() {
    if (!LittleFS.begin()) {
        ESP_LOGE(TAG, "Failed to initialize LittleFS");
    }
}

void Device::startWiFi(void) {
    auto network = _config.WiFi.SavedNetworks[_currentWiFi];

    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(network.SSID.c_str(), network.Password.c_str());

    auto result = WiFi.waitForConnectResult();
    ESP_LOGD(TAG, "WiFi begin result: %d", result);
    logWiFiResult(result);
}

void Device::stopWiFi(void) {
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
    delay(1000);
    ESP_LOGD(TAG, "WiFi stopped");
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