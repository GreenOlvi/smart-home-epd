#include <Arduino.h>
#include "device.h"
#include "epdgui/epdgui.h"
#include "frame/frame.h"

static const char *TAG = "main";

void setup()
{
    EPD.init();

    Frame_Main *main = new Frame_Main();
    EPDGUI_PushFrame(main);
}

void loop()
{
    EPDGUI_MainLoop();
}