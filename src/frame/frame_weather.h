#ifndef frame_weather_h
#define frame_weather_h

#define FrameWeatherId 2

#include "frame_base.h"
#include "epdgui/epdgui.h"
#include "frame.h"

class WeatherFrame : public Frame_Base
{
    public:
        WeatherFrame(void);
        ~WeatherFrame(void);
        int init(epdgui_args_vector_t &args);
        int run(void) override;

    private:
        M5EPD_Canvas *_frame;

        bool UpdateClock(void);
        void DrawClock(m5epd_update_mode_t mode);

        char* _clockText;
        unsigned long _clock_next_update;
        bool _clock_force_update;

        M5EPD_Canvas *_clockCanvas = NULL;
};

#endif