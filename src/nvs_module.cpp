#include "nvs_module.h"

#define TAG "NvsModule"
#define DEFAULT_NAMESPACE "store"

void NvsModule::init(void)
{
    nvs_flash_init();
}

void NvsModule::setValueU8(const char* key, const uint8_t value)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(DEFAULT_NAMESPACE, NVS_READWRITE, &handle);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error opening NVS: [0x%x] %s", err, msg);
        return;
    }

    err = nvs_set_u8(handle, key, value);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error saving to NVS: [0x%x] %s", err, msg);
        return;
    }

    err = nvs_commit(handle);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error committing to NVS: [0x%x] %s", err, msg);
        return;
    }

    nvs_close(handle);
}

bool NvsModule::tryGetValueU8(const char *key, uint8_t *value)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(DEFAULT_NAMESPACE, NVS_READONLY, &handle);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error opening NVS: [0x%x] %s", err, msg);
        return false;
    }

    err = nvs_get_u8(handle, key, value);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error reading from NVS: [0x%x] %s", err, msg);
        value = NULL;
        return false;
    }

    nvs_close(handle);

    return true;
}

uint8_t NvsModule::getValueU8(const char* key, const uint8_t defaultValue)
{
    nvs_handle handle;
    esp_err_t err = nvs_open(DEFAULT_NAMESPACE, NVS_READONLY, &handle);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error opening NVS: [0x%x] %s", err, msg);
        return defaultValue;
    }

    uint8_t value;
    err = nvs_get_u8(handle, key, &value);
    if (err)
    {
        const char* msg = esp_err_to_name(err);
        ESP_LOGE(TAG, "Error reading from NVS: [0x%x] %s", err, msg);
        return defaultValue;
    }

    nvs_close(handle);

    return value;
}