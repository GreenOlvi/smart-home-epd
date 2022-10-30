#include "config.h"

static const char *TAG = "CONFIG";
static const char *ConfigFile = "/config.json";

void readSmartHomeConfig(const JsonObject &doc, SmartHomeConfig &config) {
    config.BaseUrl = doc["BaseUrl"].as<String>();
    ESP_LOGD(TAG, "SmartHome.BaseUrl=%s", config.BaseUrl.c_str());
}

void readWiFiNetworkConfig(const JsonObject &doc, WiFiNetwork &config) {
    config.SSID = doc["SSID"].as<String>();
    ESP_LOGD(TAG, "WiFi.SavedNetworks[].SSID=%s", config.SSID.c_str());

    config.Password = doc["Password"].as<String>();
    ESP_LOGD(TAG, "WiFi.SavedNetworks[].Password=******");
}

void readWiFiConfig(const JsonObject &doc, WiFiConfig &config) {
    config.AutoConnectOnStart = doc["AutoConnectOnStart"].as<bool>();
    ESP_LOGD(TAG, "WiFi.AutoConnectOnStart=%d", config.AutoConnectOnStart);

    auto networks = doc["SavedNetworks"].as<JsonArray>();
    auto size = networks.size();
    config.SavedNetworksCount = size;
    ESP_LOGD(TAG, "WiFi.SavedNetworksCount=%d", config.SavedNetworksCount);

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
    ESP_LOGD(TAG, "Loading config from '%s' [%s]", ConfigFile, FILE_READ);
    auto file = LittleFS.open(ConfigFile, FILE_READ);
    if (!file) {
        ESP_LOGW(TAG, "Could not open config file");
        return false;
    }
    ESP_LOGV(TAG, "File opened");

    StaticJsonDocument<512> doc;

    auto deserializationError = deserializeJson(doc, file);
    if (deserializationError) {
        ESP_LOGW(TAG, "Failed to deserialize file");
        return false;
    }
    ESP_LOGV(TAG, "File deserialized");
    #if ARDUHAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_VERBOSE
    serializeJsonPretty(doc, Serial);
    #endif

    readConfig(doc.as<JsonObject>(), config);
    ESP_LOGV(TAG, "Config read");

    file.close();
    ESP_LOGV(TAG, "File closed");

    return true;
}

void writeWiFiNetworkConfig(const WiFiNetwork &config, JsonObject &doc) {
    doc["SSID"] = config.SSID;
    doc["Password"] = config.Password;
}

void writeWiFiConfig(const WiFiConfig &config, JsonObject &doc) {
    auto networks = doc.createNestedArray("SavedNetworks");
    for (auto i = 0; i < config.SavedNetworksCount; i++) {
        auto net = networks.createNestedObject();
        writeWiFiNetworkConfig(config.SavedNetworks[i], net);
    }
    doc["AutoConnectOnStart"] = config.AutoConnectOnStart;
}

void writeSmartHomeConfig(const SmartHomeConfig &config, JsonObject &doc) {
    doc["BaseUrl"] = config.BaseUrl;
}

void writeConfig(const GeneralConfiguration &config, JsonDocument &doc) {
    auto wifi = doc.createNestedObject("WiFi");
    writeWiFiConfig(config.WiFi, wifi);

    auto sh = doc.createNestedObject("SmartHome");
    writeSmartHomeConfig(config.SmartHome, sh);
}

bool saveConfiguration(const GeneralConfiguration &config) {
    ESP_LOGD(TAG, "Saving config to '%s'", ConfigFile);
    auto file = LittleFS.open(ConfigFile, FILE_WRITE);
    if (!file) {
        ESP_LOGW(TAG, "Could not open config file for writing");
        return false;
    }
    ESP_LOGV(TAG, "File opened");

    StaticJsonDocument<512> doc;

    writeConfig(config, doc);
    ESP_LOGV(TAG, "Config written");

    if (serializeJson(doc, file) == 0) {
        ESP_LOGW(TAG, "Failed to serialize config");
        return false;
    }
    ESP_LOGV(TAG, "Config serialized");

    file.close();
    ESP_LOGV(TAG, "File closed");

    return true;
}