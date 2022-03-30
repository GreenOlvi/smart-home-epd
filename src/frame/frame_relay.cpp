#include "frame_relay.h"

RelayFrame::RelayFrame(): Frame_Base(true)
{
    _frame_name = "Frame_Relay";
    exitbtn("Main");
    _canvas_title->drawString("Relays", 270, 34);

    _key_exit->AddArgs(EPDGUI_Button::EVENT_RELEASED, 0, (void*)(&_is_run));
    _key_exit->Bind(EPDGUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);

    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        _relayButtons[i] = new EPDGUI_Button(4, 100 + i * 64, 532, 61);
        _relayButtons[i]->SetHide(true);
        _relayButtons[i]->CanvasNormal()->setTextSize(5);
        _relayButtons[i]->CanvasNormal()->setTextDatum(CL_DATUM);
        _relayButtons[i]->CanvasNormal()->setTextColor(15);
    }

    _apiClient = new SmartHomeApiClient(SMART_HOME_API_BASE_URL);
    _reloadRelays = true;
}

RelayFrame::~RelayFrame()
{
    delete _apiClient;
    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        delete _relayButtons[i];
    }
}

int RelayFrame::init(epdgui_args_vector_t &args)
{
    _is_run = 1;

    M5.EPD.Clear();
    _canvas_title->pushCanvas(0, 8, UPDATE_MODE_NONE);

    EPDGUI_AddObject(_key_exit);

    for (int i = 0; i < MAX_BUTTONS; i++)
    {
        // _relayButtons[i]->SetHide(false);
        EPDGUI_AddObject(_relayButtons[i]);
    }

    return 3;
}

int RelayFrame::run()
{
    Frame_Base::run();
    M5.update();

    if (_reloadRelays)
    {
        _reloadRelays = false;
        if (_apiClient->GetRelays(_relays))
        {
            log_i("Loading relays success");
            for (auto relay : *_relays)
            {
                log_d("Id=%s, Type=%s, Name=%s", relay.Id.c_str(), relay.Type.c_str(), relay.Name.c_str());
            }

            updateRelayButtons();
        }
        else
        {
            log_w("Loading relays failed");
        }
    }

    return _is_run;
}

void RelayFrame::updateRelayButtons()
{
    int i = 0;
    for (auto r : *_relays)
    {
        drawRelayButton(_relayButtons[i], r);
        _relayButtons[i]->Draw(UPDATE_MODE_A2);
        i++;
    }

    for (; i < MAX_BUTTONS; i++)
    {
        _relayButtons[i]->SetHide(true);
    }

    M5.EPD.UpdateFull(UPDATE_MODE_GL16);
}

void RelayFrame::drawRelayButton(EPDGUI_Button *btn, const Relay r)
{
    btn->SetHide(false);
    btn->CanvasNormal()->fillCanvas(0);
    btn->CanvasNormal()->drawRect(0, 0, 532, 60, 15);
    btn->CanvasNormal()->drawString(r.Name, 15, 35);
    *(btn->CanvasPressed()) = *(btn->CanvasNormal());
    btn->CanvasPressed()->ReverseColor();
}
