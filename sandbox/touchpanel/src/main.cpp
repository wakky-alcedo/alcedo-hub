#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#define PIN_LED_RED 4
#define PIN_LED_GREEN 16
#define PIN_LED_BLUE 17
#define PIN_BRIGTHNESS 4
#define PIN_SPEAKER 26

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
};

LGFX display;
LGFX_Sprite sprite(&display);      // lcdに描画するスプライト作成

void setup(void) {
    display.init();

    display.setTextSize((std::max(display.width(), display.height()) + 255) >> 8);

    if (display.touch()) {
        if (display.width() < display.height()) {
            display.setRotation(display.getRotation() ^ 1); // 縦向きの場合は横向きにする
        }

        display.setTextDatum(textdatum_t::middle_center); // テキストの描画位置を設定
        display.drawString("touch the arrow marker.", display.width() >> 1, display.height() >> 1); // メッセージを表示
        display.setTextDatum(textdatum_t::top_left); // テキストの描画位置を設定

        // std::uint16_t fg = TFT_WHITE;
        // std::uint16_t bg = TFT_BLACK;
        // if (display.isEPD()) std::swap(fg, bg);
        // display.calibrateTouch(nullptr, fg, bg, std::max(display.width(), display.height()) >> 3);
    }

    display.fillScreen(TFT_BLACK);

    display.setBrightness(255); // 明るさの設定 なんかうまく反映されていなさそう？

    // ピンの設定
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_BLUE, OUTPUT);
    pinMode(PIN_BRIGTHNESS, INPUT);

    analogWrite(PIN_LED_RED, 0); // 明るさが逆
    analogWrite(PIN_LED_GREEN, 0);
    analogWrite(PIN_LED_BLUE, 0);

    sprite.createSprite(100, 20);
}

uint32_t count = ~0;
void loop(void) {


    // 明るさを取得
    uint16_t brightness = analogRead(34);


    sprite.fillScreen(TFT_BLACK);
    sprite.setTextColor(TFT_WHITE); // 文字色を設定
    sprite.drawString("brightness", 0, 0);
    sprite.drawNumber(brightness, 70, 0); // 画面の向きを表示

    display.startWrite();
    sprite.pushSprite(30, 50);
    display.endWrite(); // 描画を終了
}