#ifndef _FRAME_MAIN_H_
#define _FRAME_MAIN_H_

#include "frame_base.h"
#include "frame_weather.h"
#include "frame_todo.h"
#include "epdgui/epdgui.h"
#include "epdgui/epdgui_switch.h"

#include <WiFi.h>
#include "secrets.h"

class Frame_Main : public Frame_Base
{
    public:
        Frame_Main();
        ~Frame_Main();
        int run();
        int init(epdgui_args_vector_t &args);
        void updateStatusBar(void);

    private:
        M5EPD_Canvas *_bar;
        EPDGUI_Button *_weatherButton;
        EPDGUI_Button *_todoButton;
        EPDGUI_Switch *_wifiButton;
        uint32_t _next_update_time;
        uint32_t _time;

        void DrawStatusBar(m5epd_update_mode_t mode);
};

#endif //_FRAME_MAIN_H_