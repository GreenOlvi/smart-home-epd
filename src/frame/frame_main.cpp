#include "frame_main.h"
#include "frame_weather.h"
#include "frame_todo.h"

#define KEY_W 92
#define KEY_H 92

void button_weather_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Weather");
    if (frame == NULL)
    {
        frame = new WeatherFrame();
        EPDGUI_AddFrame("Frame_Weather", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void button_todo_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Todo");
    if (frame == NULL)
    {
        frame = new TodoFrame();
        EPDGUI_AddFrame("Frame_Todo", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

bool wifi = false;

void button_wifi_cb(epdgui_args_vector_t &args) {
    Serial.println("Toggle wifi");
    wifi = !wifi;
}


Frame_Main::Frame_Main(void): Frame_Base(false)
{
    _frame_name = "Frame_Main";
    _frame_id = 1;

    _bar = new M5EPD_Canvas(&M5.EPD);
    _bar->createCanvas(540, 44);
    _bar->setTextSize(2);

    _weatherButton = new EPDGUI_Button(20, 90, KEY_W, KEY_H);
    _weatherButton->CanvasNormal()->setTextSize(3);
    _weatherButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _weatherButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_weather_92x92);
    *(_weatherButton->CanvasPressed()) = *(_weatherButton->CanvasNormal());
    _weatherButton->CanvasPressed()->ReverseColor();
    _weatherButton->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _weatherButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_weather_cb);

    _todoButton = new EPDGUI_Button(132, 90, KEY_W, KEY_H);
    _todoButton->CanvasNormal()->setTextSize(3);
    _todoButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _todoButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_todo_92x92);
    *(_todoButton->CanvasPressed()) = *(_todoButton->CanvasNormal());
    _todoButton->CanvasPressed()->ReverseColor();
    _todoButton->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _todoButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_todo_cb);

    _wifiButton = new EPDGUI_Button(244, 90, KEY_W, KEY_H);
    _wifiButton->CanvasNormal()->setTextSize(3);
    _wifiButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _wifiButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_wifi_on_92x92);
    *(_wifiButton->CanvasPressed()) = *(_wifiButton->CanvasNormal());
    _wifiButton->CanvasPressed()->ReverseColor();
    _wifiButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_wifi_cb);

    _time = 0;
    _next_update_time = 0;
}


Frame_Main::~Frame_Main(void)
{
    delete _weatherButton;
    delete _todoButton;
    delete _wifiButton;
}

void Frame_Main::DrawStatusBar(m5epd_update_mode_t mode)
{
    if((millis() - _time) < _next_update_time)
    {
        return;
    }
    char buf[20];
    _bar->setTextSize(2);
    _bar->fillCanvas(0);
    _bar->drawFastHLine(0, 43, 540, 15);
    _bar->setTextDatum(CL_DATUM);
    _bar->drawString("M5Paper", 10, 27);

    // Battery
    _bar->setTextDatum(CR_DATUM);
    _bar->pushImage(498, 8, 32, 32, image_status_bar_battery_32x32);
    uint32_t vol = M5.getBatteryVoltage();

    if(vol < 3300)
    {
        vol = 3300;
    }
    else if(vol > 4350)
    {
        vol = 4350;
    }
    float battery = (float)(vol - 3300) / (float)(4350 - 3300);
    if(battery <= 0.01)
    {
        battery = 0.01;
    }
    if(battery > 1)
    {
        battery = 1;
    }
    uint8_t px = battery * 25;
    sprintf(buf, "%d%%", (int)(battery * 100));
    _bar->drawString(buf, 498 - 10, 27);
    _bar->fillRect(498 + 3, 8 + 10, px, 13, 15);

    // Time
    rtc_time_t time_struct;
    rtc_date_t date_struct;
    M5.RTC.getTime(&time_struct);
    M5.RTC.getDate(&date_struct);
    sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
    _bar->setTextDatum(CC_DATUM);
    _bar->drawString(buf, 270, 27);
    _bar->pushCanvas(0, 0, mode);

    _time = millis();
    _next_update_time = (60 - time_struct.sec) * 1000;
}

int Frame_Main::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    EPDGUI_AddObject(_weatherButton);
    EPDGUI_AddObject(_todoButton);
    EPDGUI_AddObject(_wifiButton);

    _time = 0;
    _next_update_time = 0;
    DrawStatusBar(UPDATE_MODE_NONE);
    return 9;
}

int Frame_Main::run()
{
    Frame_Base::run();
    DrawStatusBar(UPDATE_MODE_GL16);
    return 1;
}