#ifndef _FRAME_MAIN_H_
#define _FRAME_MAIN_H_

#include "frame_base.h"

class Frame_Main : public Frame_Base
{
public:
    Frame_Main();
    ~Frame_Main();
    int run();
    int init(epdgui_args_vector_t &args);
    void DrawStatusBar(m5epd_update_mode_t mode);

private:
    M5EPD_Canvas *_bar;
    uint32_t _next_update_time;
    uint32_t _time;
};

#endif //_FRAME_MAIN_H_