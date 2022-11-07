#ifndef _FRAME_MAIN_H_
#define _FRAME_MAIN_H_

#include "frame_base.h"
#include "frame_relay.h"
#include "frame_todo.h"
#include "frame_weather.h"
#include "epdgui/epdgui.h"
#include "epdgui/epdgui_switch.h"

#include <WiFi.h>
#include "device.h"

class Frame_Main : public Frame_Base
{
    public:
        Frame_Main();
        ~Frame_Main();
        int run() override;
        int init(epdgui_args_vector_t &args);
        void updateStatusBar(void);
        void wifiConnectBlocking(void);
        void wifiDisconnectBlocking(void);

    private:
        M5EPD_Canvas *_bar;
        EPDGUI_Button *_weatherButton;
        EPDGUI_Button *_todoButton;
        EPDGUI_Button *_relayButton;
        EPDGUI_Switch *_wifiButton;

        bool _statusbar_force_update;
        uint32_t _statusbar_last_draw;
        uint32_t _statusbar_next_time_update;

        bool _wifiConnect = true;

        void UpdateWiFi(void);

        void DrawStatusBarIfNeeded(m5epd_update_mode_t mode);
        void DrawStatusBar(m5epd_update_mode_t mode);
        void DrawBattery(int x, int y);

        void SetUpButtons();
        void SetUpWeatherButton(int x, int y);
        void SetUpToDoButton(int x, int y);
        void SetUpRelayButton(int x, int y);
        void SetUpWiFiButton(int x, int y);
};

#endif //_FRAME_MAIN_H_