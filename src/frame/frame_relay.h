#ifndef frame_relay_h
#define frame_relay_h

#include <SmartHomeApiClient.h>
#include "secrets.h"
#include "frame_base.h"
#include "epdgui/epdgui.h"

#define MAX_BUTTONS 10

class RelayFrame : public Frame_Base
{
    public:
        RelayFrame(void);
        ~RelayFrame(void);
        int init(epdgui_args_vector_t &args);
        int run(void) override;

    private:
        M5EPD_Canvas *_frame;
        SmartHomeApiClient *_apiClient;
        bool _reloadRelays;

        std::vector<Relay> *_relays;
        EPDGUI_Button *_relayButtons[MAX_BUTTONS];

        void updateRelayButtons(void);
        void drawRelayButton(EPDGUI_Button *btn, const Relay r);
};

#endif