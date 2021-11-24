#include "frame_base.h"
#include "../epdgui/epdgui.h"

#define ENABLE_AUTO_POWER_SAVE true
static const uint32_t TIME_BEFORE_SHUTDOWN_PROMPT_MS = 60 * 1000;
static const uint32_t SHUTDOWN_PROMPT_DELAY_MS = 10 * 1000;
static const uint32_t TIME_BEFORE_SHUTDOWN_MS = TIME_BEFORE_SHUTDOWN_PROMPT_MS + SHUTDOWN_PROMPT_DELAY_MS;

void Shutdown()
{
    log_d("Now the system is shutting down.");
    M5.EPD.Clear();
    M5.EPD.UpdateFull(UPDATE_MODE_GC16);
    // SaveSetting();
    delay(600);
    M5.shutdown();
    while(1);
}

Frame_Base::Frame_Base(bool _has_title)
{
    if(_has_title)
    {
        _canvas_title = new M5EPD_Canvas(&M5.EPD);
        _canvas_title->createCanvas(540, 64);
        _canvas_title->drawFastHLine(0, 64, 540, 15);
        _canvas_title->drawFastHLine(0, 63, 540, 15);
        _canvas_title->drawFastHLine(0, 62, 540, 15);
        _canvas_title->setTextSize(2);
        _canvas_title->setTextDatum(CC_DATUM);
    }
    _frame_id = 0;
    _frame_name = "Frame_Base";
    EPDGUI_UpdateGlobalLastActiveTime();
}

Frame_Base::~Frame_Base() 
{
    if(_key_exit != NULL)
        delete _key_exit;
    if(_canvas_title != NULL)
        delete _canvas_title;
    if (_canvas_footer != NULL)
    {
        delete _canvas_footer;
    }
}

void Frame_Base::CheckAutoPowerSave()
{
    unsigned long now = millis();
    int footer_height = 28;
    int margin_bottom = 10;
    if (now - g_last_active_time_millis > TIME_BEFORE_SHUTDOWN_MS)
    {
        log_d("The idle time reached, shutdown now.");
        Shutdown();
    }
    else if (now - g_last_active_time_millis > TIME_BEFORE_SHUTDOWN_PROMPT_MS)
    {
        if (!_shutdown_prompt_is_shown)
        {
            log_d("Show shutdown prompt");
            _canvas_footer = new M5EPD_Canvas(&M5.EPD);
            _canvas_footer->createCanvas(540, footer_height);
            _canvas_footer->setTextSize(2);
            _canvas_footer->setTextDatum(CC_DATUM);
            char buf[128];
            sprintf(buf, "Shutdown to save power, touch to continue?");
            _canvas_footer->drawString(buf, 270, footer_height / 2);
            _canvas_footer->pushCanvas(0, 960 - footer_height - margin_bottom, UPDATE_MODE_DU4);
            _shutdown_prompt_is_shown = true;
        }
    }
    else if (_shutdown_prompt_is_shown)
    {
        // active again and _shutdown_prompt_is_shown == true, hide prompt
        log_d("Become active again, hide prompt");
        _canvas_footer->fillCanvas(0);
        _canvas_footer->pushCanvas(0, 960 - footer_height - margin_bottom, UPDATE_MODE_DU4);
        _shutdown_prompt_is_shown = false;
    }
}

int Frame_Base::run(void)
{
    if (ENABLE_AUTO_POWER_SAVE)
    {
        CheckAutoPowerSave();
    }
    return _is_run;
}

void Frame_Base::exit(void)
{
}

void Frame_Base::exit_cb(epdgui_args_vector_t &args)
{
    EPDGUI_PopFrame();
    *((int*)(args[0])) = 0;
}