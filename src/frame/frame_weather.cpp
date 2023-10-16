#include "frame_weather.h"

static const char *TAG = "FrameWeather";

WeatherFrame::WeatherFrame(): Frame_Base(true)
{
    _frame_id = FrameWeatherId;
    _frame_name = "Frame_Weather";

    exitbtn("Main");
    _canvas_title->drawString("Weather", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    _clockCanvas = new M5EPD_Canvas(&M5.EPD);
    _clockCanvas->createCanvas(300, 100);
    _clockCanvas->drawFastHLine(0, 0, 300, 15);
    _clockCanvas->drawFastHLine(0, 99, 300, 15);
    _clockCanvas->drawFastVLine(0, 0, 100, 15);
    _clockCanvas->drawFastVLine(299, 0, 100, 15);
    _clockText = new char[20];
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

int WeatherFrame::run(void)
{
    if (!Frame_Base::run())
    {
        return 0;
    }

    if (UpdateClock())
    {
        DrawClock(UPDATE_MODE_GL16);
    }

    return _is_run;
}

bool WeatherFrame::UpdateClock(void)
{
    if (!(millis() > _clock_next_update || _clock_force_update))
    {
        return false;
    }

    tm time;
    getLocalTime(&time);

    _clock_next_update = millis() + (60 - time.tm_sec) * 1000;
    _clock_force_update = false;

    sprintf(_clockText, "%02d:%02d:%02d", time.tm_hour, time.tm_min, time.tm_sec);
    ESP_LOGI(TAG, "Current time %s", _clockText);

    return true;
}

void WeatherFrame::DrawClock(m5epd_update_mode_t mode)
{
    _clockCanvas->setTextDatum(CC_DATUM);
    _clockCanvas->setTextSize(4);
    _clockCanvas->drawString(_clockText, 150, 50);
    _clockCanvas->pushCanvas(200, 200, mode);
}