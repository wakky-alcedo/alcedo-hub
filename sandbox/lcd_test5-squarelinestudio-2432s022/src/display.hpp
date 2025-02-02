#include <LovyanGFX.hpp>
#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

CST820 touch(I2C_SDA, I2C_SCL, -1, -1); /* 触摸实例 */

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];


class LGFX : public lgfx::LGFX_Device
{
    lgfx::Panel_ST7789  _panel_instance;    // ST7789UI
    lgfx::Bus_Parallel8 _bus_instance;      // MCU8080 8B
    lgfx::Light_PWM _light_instance;
    lgfx::Touch_FT5x06 _touch_instance;

public:
    LGFX(void)
    {
        {
            auto cfg = _bus_instance.config();
            cfg.freq_write = 25000000;
            cfg.pin_wr = 4;
            cfg.pin_rd = 2;
            cfg.pin_rs = 16;

            cfg.pin_d0 = 15;
            cfg.pin_d1 = 13;
            cfg.pin_d2 = 12;
            cfg.pin_d3 = 14;
            cfg.pin_d4 = 27;
            cfg.pin_d5 = 25;
            cfg.pin_d6 = 33;
            cfg.pin_d7 = 32;

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs = 17;
            cfg.pin_rst = -1;
            cfg.pin_busy = -1;

            cfg.panel_width = 240;
            cfg.panel_height = 320;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            // cfg.dummy_read_pixel = 8;
            // cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = false;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};

LGFX tft;

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    // tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.writePixels((lgfx::rgb565_t *)&color_p->full, w * h );
    tft.endWrite();

    lv_disp_flush_ready( disp );
}


/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    bool touched;
    uint8_t gesture;
    uint16_t touchX, touchY;

    touched = touch.getTouch(&touchX, &touchY, &gesture);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchY;
        data->point.y = screenHeight-touchX;
    }
}

/*setup the display*/
void display_setup()
{
    touch.begin(); /*初始化触摸板*/

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

    tft.begin();          /* TFT init */
    tft.setRotation( 1 ); /* Landscape orientation, flipped */

    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );

    /*Initialize the UI*/
    ui_init();
    // Light
    lv_obj_set_style_text_font(ui_LabelHome1, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome1, LV_SYMBOL_NEW_LINE);
    // A/C
    lv_obj_set_style_text_font(ui_LabelHome2, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome2, LV_SYMBOL_NEW_LINE);
    lv_label_set_text( ui_LabelTempUp, LV_SYMBOL_UP);
    lv_label_set_text( ui_LabelTempDown, LV_SYMBOL_DOWN);
    lv_obj_set_style_text_font(ui_LabelACOn, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelACOn, LV_SYMBOL_POWER);
    lv_obj_set_style_text_font(ui_LabelACOff, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_label_set_text( ui_LabelACOff, LV_SYMBOL_CLOSE);
    lv_label_set_text( ui_LabelACOff, "OFF");
    // Curtain
    lv_obj_set_style_text_font(ui_LabelHome3, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome3, LV_SYMBOL_NEW_LINE);
    lv_obj_set_style_text_font(ui_LabelOpen, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelOpen, LV_SYMBOL_LEFT LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(ui_LabelStop, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelStop, LV_SYMBOL_PAUSE);
    lv_obj_set_style_text_font(ui_LabelClose, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelClose, LV_SYMBOL_RIGHT LV_SYMBOL_LEFT);
}

/*Display loop*/
void display_loop()
{
    lv_task_handler();
}

#endif // DISPLAY_HPP_
