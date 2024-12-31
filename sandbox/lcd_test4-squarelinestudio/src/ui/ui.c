// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Home
void ui_Home_screen_init(void);
lv_obj_t * ui_Home;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
lv_obj_t * ui_Temperture;
lv_obj_t * ui_Label3;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Temperture1;
lv_obj_t * ui_Label1;
lv_obj_t * ui_Label2;
lv_obj_t * ui_Temperture2;
lv_obj_t * ui_Label8;
lv_obj_t * ui_Label9;
// CUSTOM VARIABLES
lv_obj_t * uic_sample;


// SCREEN: ui_Screen2
void ui_Screen2_screen_init(void);
lv_obj_t * ui_Screen2;
lv_obj_t * ui_Button1;
void ui_event_SwitchLight(lv_event_t * e);
lv_obj_t * ui_SwitchLight;
void ui_event_ButtonHome1(lv_event_t * e);
lv_obj_t * ui_ButtonHome1;
lv_obj_t * ui_LabelHome1;
lv_obj_t * ui_Container1;
void ui_event_SliderBrightness(lv_event_t * e);
lv_obj_t * ui_SliderBrightness;
lv_obj_t * ui_LabelBrightness;
lv_obj_t * ui_Container2;
void ui_event_SliderColor(lv_event_t * e);
lv_obj_t * ui_SliderColor;
lv_obj_t * ui_LabelColor;
lv_obj_t * ui_Label6;
// CUSTOM VARIABLES
lv_obj_t * uic_aa;


// SCREEN: ui_Screen3
void ui_Screen3_screen_init(void);
lv_obj_t * ui_Screen3;
void ui_event_ButtonHome2(lv_event_t * e);
lv_obj_t * ui_ButtonHome2;
lv_obj_t * ui_Dropdown1;
lv_obj_t * ui_Button4;
lv_obj_t * ui_Panel2;
lv_obj_t * ui_LabelTemp;
void ui_event_ButtonTempUp(lv_event_t * e);
lv_obj_t * ui_ButtonTempUp;
lv_obj_t * ui_LabelTempUp;
void ui_event_ButtonTempDown(lv_event_t * e);
lv_obj_t * ui_ButtonTempDown;
lv_obj_t * ui_LabelTempDown;
lv_obj_t * ui_ButtonACOn;
lv_obj_t * ui_ButtonACOff;
lv_obj_t * ui_Label4;
// CUSTOM VARIABLES


// SCREEN: ui_Screen4
void ui_Screen4_screen_init(void);
lv_obj_t * ui_Screen4;
void ui_event_ButtonHome3(lv_event_t * e);
lv_obj_t * ui_ButtonHome3;
lv_obj_t * ui_Container4;
void ui_event_SliderBrightness2(lv_event_t * e);
lv_obj_t * ui_SliderBrightness2;
lv_obj_t * ui_LabelBrightness2;
lv_obj_t * ui_Container6;
void ui_event_ButtonOpen(lv_event_t * e);
lv_obj_t * ui_ButtonOpen;
lv_obj_t * ui_LabelOpen;
void ui_event_ButtonStop(lv_event_t * e);
lv_obj_t * ui_ButtonStop;
lv_obj_t * ui_LabelStop;
void ui_event_ButtonClose(lv_event_t * e);
lv_obj_t * ui_ButtonClose;
lv_obj_t * ui_LabelClose;
lv_obj_t * ui_Label7;
// CUSTOM VARIABLES


// SCREEN: ui_Screen5
void ui_Screen5_screen_init(void);
lv_obj_t * ui_Screen5;
lv_obj_t * ui_SpinboxTemp2;
// CUSTOM VARIABLES

// EVENTS
lv_obj_t * ui____initial_actions0;

// IMAGES AND IMAGE SETS

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen2, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 0, &ui_Screen2_screen_init);
    }
}

void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen3, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen3_screen_init);
    }
}

void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Screen4, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Screen4_screen_init);
    }
}

void ui_event_SwitchLight(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        SwitchLightClicked(e);
    }
}

void ui_event_ButtonHome1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Home, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 500, 0, &ui_Home_screen_init);
    }
}

void ui_event_SliderBrightness(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        SliderBrightnessChanged(e);
    }
}

void ui_event_SliderColor(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        SliderColorChanged(e);
    }
}

void ui_event_ButtonHome2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Home_screen_init);
    }
}

void ui_event_ButtonTempUp(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        ButtonTempUpClicked(e);
    }
}

void ui_event_ButtonTempDown(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        ButtonTempDownClicked(e);
    }
}

void ui_event_ButtonHome3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_Home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Home_screen_init);
    }
}

void ui_event_SliderBrightness2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_VALUE_CHANGED) {
        SliderBrightnessChanged(e);
    }
}

void ui_event_ButtonOpen(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        ButtonOpenClicked(e);
    }
}

void ui_event_ButtonStop(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        ButtonStopClicked(e);
    }
}

void ui_event_ButtonClose(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);

    if(event_code == LV_EVENT_CLICKED) {
        ButtonCloseClicked(e);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Home_screen_init();
    ui_Screen2_screen_init();
    ui_Screen3_screen_init();
    ui_Screen4_screen_init();
    ui_Screen5_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_Home);
}
