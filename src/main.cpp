#include <Arduino.h>
#include <M5EPD.h>
#include "epdgui/epdgui.h"
#include "frame/frame.h"

M5EPD_Canvas canvas(&M5.EPD);

void setup() {
    M5.begin(true, false, true, true, false);

    M5.EPD.SetRotation(M5EPD_Driver::ROTATE_90);
    M5.EPD.Clear(true);
    M5.TP.SetRotation(GT911::ROTATE_90);

    Frame_Main *main = new Frame_Main();
    EPDGUI_PushFrame(main);
}

void loop() {
    EPDGUI_MainLoop();
}