#include <lvgl.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include <display.hpp>
#include <ui/ui.h>

/*Don't forget to set Sketchbook location in File/Preferences to the path of your UI project (the parent foder of this INO file)*/

/*Change to your screen resolution*/
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

LGFX tft;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

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
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX = 0, touchY = 0;

    bool touched = tft.getTouch( &touchX, &touchY);

    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;

        Serial.print( "Data x " );
        Serial.println( touchX );

        Serial.print( "Data y " );
        Serial.println( touchY );
    }
}

void setup()
{
    Serial.begin( 115200 ); /* prepare for possible serial debug */

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );
    Serial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

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


    ui_init();
    // Light
    lv_obj_set_style_text_font(ui_LabelHome1, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelHome1, LV_SYMBOL_NEW_LINE);
    // A/C
    lv_label_set_text( ui_LabelTempUp, LV_SYMBOL_UP);
    lv_label_set_text( ui_LabelTempDown, LV_SYMBOL_DOWN);
    // Curtain
    lv_obj_set_style_text_font(ui_LabelOpen, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelOpen, LV_SYMBOL_LEFT LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(ui_LabelStop, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelStop, LV_SYMBOL_PAUSE);
    lv_obj_set_style_text_font(ui_LabelClose, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text( ui_LabelClose, LV_SYMBOL_RIGHT LV_SYMBOL_LEFT);

    Serial.println( "Setup done" );
}

void loop()
{
    lv_timer_handler(); /* let the GUI do its work */

    // sprintf(chbuff, "%02d:%02d", tm->tm_hour, tm->tm_min);
    // lv_label_set_text(uic_sample, chbuff);
    // lv_label_set_text(uic_aa, chbuff);
    // lv_label_get

    delay(5);
}

