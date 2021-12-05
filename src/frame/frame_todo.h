#ifndef frame_todo_h
#define frame_todo_h

#include "frame_base.h"
#include "epdgui/epdgui.h"

class TodoFrame : public Frame_Base
{
    public:
        TodoFrame(void);
        ~TodoFrame(void);
        int init(epdgui_args_vector_t &args);

    private:
        M5EPD_Canvas *_frame;
};

#endif