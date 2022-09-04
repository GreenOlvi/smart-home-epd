#include <M5EPD.h>
#include "config.h"
#include "epdgui/epdgui.h"
#include "frame/frame.h"

static const char *TAG = "EPD";
M5EPD_Canvas canvas(&M5.EPD);
GeneralConfiguration config;

void mountFs() {
    if (!LittleFS.begin()) {
        ESP_LOGE(TAG, "Failed to initialize LittleFS");
    }
}

void setup() {
    M5.begin(true, false, true, true, false);

    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    M5.EPD.Clear(true);
    M5.TP.SetRotation(GT911::ROTATE_90);

    mountFs();
    loadConfiguration(config);

    Frame_Main *main = new Frame_Main();
    EPDGUI_PushFrame(main);
}

void loop() {
    EPDGUI_MainLoop();
}