#include "config.h"

static const char *TAG = "CONFIG";

void readSmartHomeConfig(const JsonObject &doc, SmartHomeConfig &config) {
    strlcpy(config.BaseUrl, doc["BaseUrl"], sizeof(config.BaseUrl));
    ESP_LOGV(TAG, "SmartHome.BaseUrl=%s", config.BaseUrl);
}

void readWiFiNetworkConfig(const JsonObject &doc, WiFiNetwork config) {
    strlcpy(config.SSID, doc["SSID"], sizeof(config.SSID));
    ESP_LOGV(TAG, "WiFi.SavedNetworks[].SSID=%s", config.SSID);
    strlcpy(config.Password, doc["Password"], sizeof(config.Password));
    ESP_LOGV(TAG, "WiFi.SavedNetworks[].Password=*****");
}

void readWiFiConfig(const JsonObject &doc, WiFiConfig &config) {
    config.AutoConnectOnStart = doc["AutoConnectOnStart"].as<bool>();
    ESP_LOGV(TAG, "WiFi.AutoConnectOnStart=%d", config.AutoConnectOnStart);

    auto networks = doc["SavedNetworks"].as<JsonArray>();
    auto size = networks.size();
    config.SavedNetworksCount = size;
    ESP_LOGV(TAG, "WiFi.SavedNetworksCount=%d", config.SavedNetworksCount);

    config.SavedNetworks = new WiFiNetwork[size];
    for (auto i = 0; i < size; i++) {
        readWiFiNetworkConfig(networks[i].as<JsonObject>(), config.SavedNetworks[i]);
    }
}

void readConfig(const JsonObject &doc, GeneralConfiguration &config) {
    readSmartHomeConfig(doc["SmartHome"].as<JsonObject>(), config.SmartHome);
    readWiFiConfig(doc["WiFi"].as<JsonObject>(), config.WiFi);
}

bool loadConfiguration(GeneralConfiguration &config) {
    ESP_LOGD(TAG, "Loading config from '%s'", ConfigFile);
    auto file = LittleFS.open(ConfigFile, "r");
    if (!file) {
        ESP_LOGW(TAG, "Could not open config file");
        return false;
    }
    ESP_LOGV(TAG, "File opened");

    StaticJsonDocument<512> doc;

    auto deserializationError = deserializeJson(doc, file);
    if (deserializationError) {
        ESP_LOGW(TAG, "Failed to read file");
        return false;

    }
    ESP_LOGV(TAG, "File deserialized");

    readConfig(doc.as<JsonObject>(), config);
    ESP_LOGV(TAG, "Config read");

    file.close();
    ESP_LOGV(TAG, "File closed");
}