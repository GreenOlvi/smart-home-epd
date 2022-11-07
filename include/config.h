#ifndef CONFIG_H
#define CONFIG_H

#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

struct WiFiNetwork {
    String SSID;
    String Password;
};

struct WiFiConfig {
    WiFiNetwork *SavedNetworks;
    int SavedNetworksCount;
    bool AutoConnectOnStart;
};

struct SmartHomeConfig {
    String BaseUrl;
};

struct GeneralConfiguration {
    WiFiConfig WiFi;
    SmartHomeConfig SmartHome;
    String NtpServer;
    String Timezone;
};

bool loadConfiguration(GeneralConfiguration &config);
bool saveConfiguration(const GeneralConfiguration &config);
void writeConfig(const GeneralConfiguration &config, JsonDocument &doc);

#endif