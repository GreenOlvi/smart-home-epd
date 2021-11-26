#include "frame_weather.h"

WeatherFrame::WeatherFrame(): Frame_Base(true)
{
    _frame_name = "Frame_Weather";
    exitbtn("Main");
    _canvas_title->drawString("Weather", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

WeatherFrame::~WeatherFrame()
{
}

int WeatherFrame::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

    EPDGUI_AddObject(_key_exit);

    return 3;
}