#include "frame_todo.h"

TodoFrame::TodoFrame(): Frame_Base(true)
{
    _frame_name = "Frame_Todo";
    exitbtn("Main");
    _canvas_title->drawString("To Do", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

TodoFrame::~TodoFrame()
{
}

int TodoFrame::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

    EPDGUI_AddObject(_key_exit);

    return 3;
}