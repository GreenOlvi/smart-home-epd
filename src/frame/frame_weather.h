#ifndef frame_weather_h
#define frame_weather_h

#include "frame_base.h"
#include "epdgui/epdgui.h"

class WeatherFrame : public Frame_Base
{
    public:
        WeatherFrame(void);
        ~WeatherFrame(void);
        int init(epdgui_args_vector_t &args);

    private:
        M5EPD_Canvas *_frame;
};

#endif