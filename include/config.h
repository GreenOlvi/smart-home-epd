#ifndef CONFIG_H
#define CONFIG_H

#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

static const char *ConfigFile = "/config.json";

struct WiFiNetwork {
    char SSID[128];
    char Password[128];
};

struct WiFiConfig {
    WiFiNetwork *SavedNetworks;
    int SavedNetworksCount;
    bool AutoConnectOnStart;
};

struct SmartHomeConfig {
    char BaseUrl[128];
};

struct GeneralConfiguration {
    WiFiConfig WiFi;
    SmartHomeConfig SmartHome;
};

bool loadConfiguration(GeneralConfiguration &config);
bool saveConfiguration(const GeneralConfiguration &config);

#endif