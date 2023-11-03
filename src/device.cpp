#include "device.h"

static const char *TAG = "EPD";
static const char *RootCAFile = "/root.pem";

Device EPD = Device();

void Device::init() {
    ESP_LOGD(TAG, "EPD: init()");

    M5.begin(true, false, true, true, false);
    ESP_LOGD(TAG, "M5 started");

    M5.RTC.begin();
    M5.RTC.disableIRQ();

    initDisplay();

    mountFs();
    ESP_LOGI(TAG, "Mounted LittleFS");

    loadConfiguration(_config);

    Nvs.init();
    Time.init(&_config);
    Time.trySetClockFromRtc();
}

void Device::initDisplay()
{
    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    M5.EPD.Clear(true);
    M5.TP.SetRotation(GT911::ROTATE_90);
    ESP_LOGD(TAG, "Display initialized");
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

bool Device::isWiFiConnected(void) {
    return WiFi.isConnected();
}

WiFiClient* Device::getWiFiClient(void) {
    if (!_wifiClient) {
        auto client = new WiFiClientSecure();
        client->setCACert(getRootCA());
        _wifiClient = client;
    }
    return _wifiClient;
}

char* Device::getRootCA(void) {
    if (!_rootCA) {
        auto file = LittleFS.open(RootCAFile, FILE_READ);
        if (!file) {
            ESP_LOGW(TAG, "Could not open root CA file");
            return nullptr;
        }
        ESP_LOGV(TAG, "File opened");
        String data = file.readString();
        _rootCA = new char[data.length()];
        strcpy(_rootCA, data.c_str());
        ESP_LOGV(TAG, "Root CA = %s", _rootCA);
        file.close();
    }
    return _rootCA;
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
