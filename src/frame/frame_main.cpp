#include "frame_main.h"

#define KEY_W 92
#define KEY_H 92

#define BUTTON_STATE_WIFI_ON 0
#define BUTTON_STATE_WIFI_OFF 1

static const char *TAG = "FrameMain";

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

void button_relay_cb(epdgui_args_vector_t &args) {
    Frame_Base *frame = EPDGUI_GetFrame("Frame_Relay");
    if (frame == NULL)
    {
        frame = new RelayFrame();
        EPDGUI_AddFrame("Frame_Relay", frame);
    }
    EPDGUI_PushFrame(frame);
    *((int*)(args[0])) = 0;
}

void button_wifi_on_cb(epdgui_args_vector_t &args) {
    Serial.println("wifi on");

    ((Frame_Main*)(args[1]))->wifiConnectBlocking();

    EPDGUI_Switch* button = (EPDGUI_Switch*)(args[0]);
    button->setState(BUTTON_STATE_WIFI_ON);
    ((Frame_Main*)(args[1]))->updateStatusBar();
}

void button_wifi_off_cb(epdgui_args_vector_t &args) {
    Serial.println("wifi off");

    ((Frame_Main*)(args[1]))->wifiDisconnectBlocking();

    EPDGUI_Switch* button = (EPDGUI_Switch*)(args[0]);
    button->setState(BUTTON_STATE_WIFI_OFF);
    ((Frame_Main*)(args[1]))->updateStatusBar();
}

Frame_Main::Frame_Main(void): Frame_Base(false)
{
    _frame_name = "Frame_Main";
    _frame_id = 1;

    _bar = new M5EPD_Canvas(&M5.EPD);
    _bar->createCanvas(540, 44);
    _bar->setTextSize(2);

    SetUpButtons();

    _statusbar_next_time_update = 0;
}

void Frame_Main::SetUpButtons()
{
    SetUpWeatherButton(20, 90);
    SetUpToDoButton(132, 90);
    SetUpRelayButton(244, 90);
    SetUpWiFiButton(356, 90);
}

void Frame_Main::SetUpWeatherButton(int x, int y)
{
    _weatherButton = new EPDGUI_Button(x, y, KEY_W, KEY_H);
    _weatherButton->CanvasNormal()->setTextSize(3);
    _weatherButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _weatherButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_weather_92x92);
    *(_weatherButton->CanvasPressed()) = *(_weatherButton->CanvasNormal());
    _weatherButton->CanvasPressed()->ReverseColor();
    _weatherButton->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _weatherButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_weather_cb);
}

void Frame_Main::SetUpToDoButton(int x, int y)
{
    _todoButton = new EPDGUI_Button(x, y, KEY_W, KEY_H);
    _todoButton->CanvasNormal()->setTextSize(3);
    _todoButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _todoButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_todo_92x92);
    *(_todoButton->CanvasPressed()) = *(_todoButton->CanvasNormal());
    _todoButton->CanvasPressed()->ReverseColor();
    _todoButton->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _todoButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_todo_cb);
}

void Frame_Main::SetUpRelayButton(int x, int y)
{
    _relayButton = new EPDGUI_Button(x, y, KEY_W, KEY_H);
    _relayButton->CanvasNormal()->setTextSize(3);
    _relayButton->CanvasNormal()->setTextDatum(CC_DATUM);
    _relayButton->CanvasNormal()->pushImage(0, 0, 92, 92, image_button_relay_92x92);
    *(_relayButton->CanvasPressed()) = *(_relayButton->CanvasNormal());
    _relayButton->CanvasPressed()->ReverseColor();
    _relayButton->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _relayButton->Bind(EPDGUI_Button::EVENT_RELEASED, button_relay_cb);
}

void Frame_Main::SetUpWiFiButton(int x, int y)
{
    _wifiButton = new EPDGUI_Switch(2, x, y, KEY_W, KEY_H);
    _wifiButton->Canvas(BUTTON_STATE_WIFI_ON)->fillCanvas(0);
    _wifiButton->Canvas(BUTTON_STATE_WIFI_ON)->pushImage(0, 0, 92, 92, image_button_wifi_on_92x92);
    _wifiButton->AddArgs(BUTTON_STATE_WIFI_ON, 0, (void*)_wifiButton);
    _wifiButton->AddArgs(BUTTON_STATE_WIFI_ON, 1, (void*)this);
    _wifiButton->Bind(BUTTON_STATE_WIFI_ON, button_wifi_on_cb);

    _wifiButton->Canvas(BUTTON_STATE_WIFI_OFF)->fillCanvas(0);
    _wifiButton->Canvas(BUTTON_STATE_WIFI_OFF)->pushImage(0, 0, 92, 92, image_button_wifi_off_92x92);
    _wifiButton->AddArgs(BUTTON_STATE_WIFI_OFF, 0, (void*)_wifiButton);
    _wifiButton->AddArgs(BUTTON_STATE_WIFI_OFF, 1, (void*)this);
    _wifiButton->Bind(BUTTON_STATE_WIFI_OFF, button_wifi_off_cb);

    if (WiFi.isConnected()) {
        _wifiButton->setState(BUTTON_STATE_WIFI_ON);
    } else {
        _wifiButton->setState(BUTTON_STATE_WIFI_OFF);
    }
}

Frame_Main::~Frame_Main(void)
{
    delete _weatherButton;
    delete _todoButton;
    delete _relayButton;
    delete _wifiButton;
}

void Frame_Main::DrawStatusBarIfNeeded(m5epd_update_mode_t mode) {
    if (!(millis() > _statusbar_next_time_update || _statusbar_force_update))
    {
        return;
    }

    DrawStatusBar(mode);

    _statusbar_last_draw = millis();
    _statusbar_force_update = false;
    ESP_LOGD(TAG, "Last statusbar draw = %d", _statusbar_last_draw);
}

void Frame_Main::DrawStatusBar(m5epd_update_mode_t mode)
{
    _bar->setTextSize(2);
    _bar->fillCanvas(0);
    _bar->drawFastHLine(0, 43, 540, 15);
    _bar->setTextDatum(CL_DATUM);
    _bar->drawString("M5Paper", 10, 27);

    // WiFi
    if (WiFi.isConnected()) {
        _bar->pushImage(400, 8, 32, 32, image_status_bar_wifi_32x32);
    }

    DrawBattery(498, 8);

    // Time
    char buf[20];
    rtc_time_t time_struct;
    M5.RTC.getTime(&time_struct);

    // Set next update before drawing
    _statusbar_next_time_update = millis() + (60 - time_struct.sec) * 1000;

    sprintf(buf, "%2d:%02d", time_struct.hour, time_struct.min);
    _bar->setTextDatum(CC_DATUM);
    _bar->drawString(buf, 270, 27);
    _bar->pushCanvas(0, 0, mode);

    ESP_LOGD(TAG, "Next statusbar time update = %d", _statusbar_next_time_update);
}

void Frame_Main::DrawBattery(int x, int y)
{
    _bar->setTextDatum(CR_DATUM);
    _bar->pushImage(x, y, 32, 32, image_status_bar_battery_32x32);
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

    char buf[20];
    sprintf(buf, "%d%%", (int)(battery * 100));

    uint8_t px = battery * 25;
    _bar->drawString(buf, x - 10, y + 19);
    _bar->fillRect(x + 3, y + 10, px, 13, 15);
}

int Frame_Main::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    EPDGUI_AddObject(_weatherButton);
    EPDGUI_AddObject(_todoButton);
    EPDGUI_AddObject(_relayButton);
    EPDGUI_AddObject(_wifiButton);

    _statusbar_next_time_update = 0;
    DrawStatusBar(UPDATE_MODE_GL16);
    return 9;
}

int Frame_Main::run()
{
    Frame_Base::run();
    DrawStatusBarIfNeeded(UPDATE_MODE_GL16);
    UpdateWiFi();
    return 1;
}

void Frame_Main::updateStatusBar() {
    _statusbar_force_update = true;
    ESP_LOGD(TAG, "Force update statusbar = true");
}

void Frame_Main::UpdateWiFi()
{
    if (EPD.isWiFiConnected() != _wifiConnect)
    {
        if (_wifiConnect)
        {
            wifiConnectBlocking();
            _wifiButton->setState(BUTTON_STATE_WIFI_ON);
        }
        else
        {
            wifiDisconnectBlocking();
            _wifiButton->setState(BUTTON_STATE_WIFI_OFF);
        }

        updateStatusBar();
        _wifiButton->Draw(UPDATE_MODE_A2);
    }
}

void Frame_Main::wifiConnectBlocking() {
    if (EPD.startWiFi()) {
        EPD.updateClock();
    }
}

void Frame_Main::wifiDisconnectBlocking() {
    EPD.stopWiFi();
}
