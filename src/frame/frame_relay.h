#ifndef frame_relay_h
#define frame_relay_h

#include "frame_base.h"
#include "epdgui/epdgui.h"

class RelayFrame : public Frame_Base
{
    public:
        RelayFrame(void);
        ~RelayFrame(void);
        int init(epdgui_args_vector_t &args);

    private:
        M5EPD_Canvas *_frame;
};

#endif