#include <LovyanGFX.hpp>
#ifndef DISPLAY_HPP_
#define DISPLAY_HPP_

class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_ILI9341 _panel_instance;
    lgfx::Bus_SPI       _bus_instance;
    lgfx::Light_PWM     _light_instance;
    lgfx::Touch_XPT2046 _touch_instance;
public:
    LGFX(void) {
        {
            auto cfg = _bus_instance.config();

            // SPIバスの設定
            cfg.spi_host         = VSPI_HOST;
            cfg.spi_mode         = 0;
            cfg.freq_write       = 40000000;
            cfg.freq_read        = 16000000;
            cfg.spi_3wire        = true;
            cfg.use_lock         = true;
            cfg.dma_channel      = SPI_DMA_CH_AUTO;
            cfg.pin_sclk         = 14; // 変更
            cfg.pin_mosi         = 13; // 変更
            cfg.pin_miso         = 12; // 変更
            cfg.pin_dc           =  2; // 変更

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();

            cfg.pin_cs           =    15; // 変更
            cfg.pin_rst          =    -1; // 変更
            cfg.pin_busy         =    -1; // 変更

            cfg.panel_width      =   240;
            cfg.panel_height     =   320;
            cfg.offset_x         =     0;
            cfg.offset_y         =     0;
            cfg.offset_rotation  =     0;
            cfg.dummy_read_pixel =     8;
            cfg.dummy_read_bits  =     1;
            cfg.readable         =  true;
            cfg.invert           = false;
            cfg.rgb_order        = false;
            cfg.dlen_16bit       = false;
            cfg.bus_shared       = false; // 変更

            _panel_instance.config(cfg);
        }

        {
            auto cfg = _light_instance.config();

            cfg.pin_bl = 21;              // 変更
            cfg.invert = false;
            cfg.freq   = 44100;           
            cfg.pwm_channel = 7;

            _light_instance.config(cfg);
            _panel_instance.setLight(&_light_instance);
        }

        {
            auto cfg = _touch_instance.config();

            cfg.x_min      =  300;  // 変更
            cfg.x_max      = 3900;  // 変更
            cfg.y_min      = 3700;  // 変更
            cfg.y_max      =  200;  // 変更
            cfg.pin_int    = -1;    // 変更
            cfg.bus_shared = false; // 変更
            cfg.offset_rotation = 0;

            cfg.spi_host = HSPI_HOST; // 変更
            cfg.freq = 1000000;
            cfg.pin_sclk = 25;        // 変更
            cfg.pin_mosi = 32;        // 変更
            cfg.pin_miso = 39;        // 変更
            cfg.pin_cs   = 33;        // 変更

            _touch_instance.config(cfg);
            _panel_instance.setTouch(&_touch_instance);
        }

        setPanel(&_panel_instance);
    }

    void draw_slider(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t ratio) {
        // fillRoundRect(x, y, width, height, height/2, TFT_WHITE); // 外枠
        // fillRoundRect(x+2, y+2, width-4, height-4, (height-4)/2, TFT_BLACK); // 内側の黒い部分
        // fillRoundRect(x+2, y+2, (width-4)*ratio/100, height-4, (height-4)/2, TFT_WHITE); // バー
        // fillCircle(x+2+(width-4)*ratio/100, y+height/2, height/2-2, TFT_WHITE); // バーの丸い部分
        // drawString(String(ratio), x+width+5, y+height/2); // バーの割合

        drawRoundRect(x, y, width+height-4, height, height/2, TFT_WHITE); // 外枠
        // fillRoundRect(x, y, width+height-4, height, height/2, TFT_WHITE); // 外枠
        // fillRoundRect(x+2, y+2, width-4+height-4, height-4, (height-4)/2, TFT_BLACK); // 内側の黒い部分
        fillRoundRect(x+2, y+2, (height-4) + (width-4)*ratio/100, height-4, (height-4)/2, TFT_WHITE); // バー
        fillRoundRect(x+2, y+2, (height-4) + (width-4)*ratio/100, height-4, (height-4)/2, TFT_WHITE); // バー
        fillCircle(x+height/2+(width)*ratio/100, y+height/2, height*1.2/2, TFT_WHITE); // バーの丸い部分
        setFont(&fonts::Font2);
        // 右寄せ3桁
        if(ratio < 10){
            drawString(String(ratio), x+width+height+5+textWidth(String(0))*2, y+height/2.0f-fontHeight()/2.0f); // バーの割合
        }else if(ratio < 100){
            drawString(String(ratio), x+width+height+5+textWidth(String(0)),   y+height/2.0f-fontHeight()/2.0f); // バーの割合
        }else{
            drawString(String(ratio), x+width+height+5,                        y+height/2.0f-fontHeight()/2.0f); // バーの割合
        }
    }

    void draw_textbox(uint16_t x, uint16_t y, uint16_t width, uint16_t height, String text) {
        fillRoundRect(x, y, width, height, height/4, TFT_WHITE); // 外枠
        fillRoundRect(x+2, y+2, width-4, height-4, (height-4)/4, TFT_BLACK); // 内側の黒い部分
        setFont(&fonts::Font2);
        drawString(text, x+width/2-textWidth(text)/2, y+height/2.0f-fontHeight()/2.0f); // テキスト
    }
};

#endif // DISPLAY_HPP_
