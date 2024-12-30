#ifndef ICON_HPP_
#define ICON_HPP_

#include <LovyanGFX.hpp>

void draw_light_bulb_icon(LGFX_Sprite& sprite, int width, int height) {
    sprite.createSprite(width+1, height); // 電球のスプライトを作成
    sprite.fillScreen(TFT_BLACK);
    
    // 電球の外枠 (円)
    int radius = width / 2;
    sprite.drawCircle(radius, radius, radius, TFT_WHITE);

    // 電球の反射 (小さい弧)
    // int reflectOffsetX = radius / 2;
    // int reflectOffsetY = radius / 3;
    // sprite.fillCircle(radius - reflectOffsetX, reflectOffsetY, radius / 6, TFT_WHITE);

    // フィラメントの描画
    int filamentWidth = radius / 2;
    int filamentHeight = radius / 3;
    int filamentStartX = radius - filamentWidth / 2;
    int filamentStartY = radius;
    sprite.drawLine(filamentStartX, filamentStartY, filamentStartX + filamentWidth, filamentStartY, TFT_WHITE);
    sprite.drawLine(filamentStartX, filamentStartY, filamentStartX + filamentWidth / 4, filamentStartY + filamentHeight, TFT_WHITE);
    sprite.drawLine(filamentStartX + filamentWidth, filamentStartY, filamentStartX + filamentWidth * 3 / 4, filamentStartY + filamentHeight, TFT_WHITE);

    // 下部のネジ部分 (複数の長方形)
    int screwHeight = height / 10;
    int screwWidth = width * 3 / 5;
    int screwX = (width - screwWidth) / 2;
    // int screwY = height - screwHeight * 3;
    int screwY = width * 0.95f;
    for (int i = 0; i < 3; i++) {
        sprite.fillRect(screwX, screwY + i * screwHeight, screwWidth, screwHeight / 2, TFT_WHITE);
    }
    sprite.fillRect((width - screwWidth/3) / 2, screwY + 3 * screwHeight, screwWidth / 3, screwHeight / 2, TFT_WHITE);
}


// void display_ac_icon(LGFX_Sprite& sprite, int width = 120, int height = 40) {
//     // エアコン本体の描画
//     int x = 0;       // 本体の左上X座標
//     int y = 0;        // 本体の左上Y座標
//     // int width = 120;   // 本体の幅
//     // int height = 40;   // 本体の高さ

//     sprite.createSprite(width, height+20); // エアコンのスプライトを作成
//     sprite.fillScreen(TFT_BLACK);

//     sprite.fillRect(x, y, width, height, TFT_WHITE);              // 本体の矩形
//     sprite.fillRect(x + 10, y + 10, width - 20, height - 20, TFT_BLACK); // 内側の黒い部分（装飾）

//     // 吹き出し口の描画
//     int ventX = x + 10;             // 吹き出し口の左上X座標
//     int ventY = y + height - 10;    // 吹き出し口の左上Y座標
//     int ventWidth = width - 20;     // 吹き出し口の幅
//     int ventHeight = 5;             // 吹き出し口の高さ
//     sprite.fillRect(ventX, ventY, ventWidth, ventHeight, TFT_WHITE);

//     // 気流の描画
//     int airX = ventX + ventWidth / 4;  // 気流の開始位置X座標
//     int airY = ventY + 10;             // 気流の開始位置Y座標
//     int airLength = ventWidth / 2;     // 気流の長さ

//     sprite.drawLine(airX, airY, airX + airLength, airY + 10, TFT_CYAN); // 気流のライン1
//     sprite.drawLine(airX, airY + 5, airX + airLength, airY + 15, TFT_CYAN); // 気流のライン2
//     sprite.drawLine(airX, airY + 10, airX + airLength, airY + 20, TFT_CYAN); // 気流のライン3
// }

// エアコンアイコンを描画する関数
void display_ac_icon(LGFX_Sprite& sprite, int width = 120, int height = 40) {
    sprite.createSprite(width, height+20); // エアコンのスプライトを作成
    sprite.fillScreen(TFT_BLACK);

    // 外枠（エアコン本体）の描画
    sprite.drawRect(0, 0, width, height, TFT_WHITE);

    // 吹き出し口の描画
    int ventX = width / 4;            // 吹き出し口の左上X座標
    int ventY = height * 3 / 4;       // 吹き出し口の左上Y座標
    int ventWidth = width / 2;            // 吹き出し口の幅
    int ventHeight = height / 8;          // 吹き出し口の高さ
    sprite.fillRect(ventX, ventY, ventWidth, ventHeight, TFT_WHITE);
}

// void display_ac_icon(LGFX_Sprite *sprite, int width, int height) {
//     // エアコン本体の描画
//     sprite.fillRect(0, 0, width, height, TFT_WHITE);                      // 本体の矩形
//     sprite.fillRect(10, 10, width - 20, height - 20, TFT_BLACK);  // 内側の黒い部分

//     // 吹き出し口の描画
//     int ventX = 10;
//     int ventY = height - 10;
//     int ventWidth = width - 20;
//     int ventHeight = 5;
//     sprite.fillRect(ventX, ventY, ventWidth, ventHeight, TFT_WHITE);

//     // 気流の描画
//     int airX = ventX + ventWidth / 4;
//     int airY = ventY + 10;
//     int airLength = ventWidth / 2;
//     sprite.drawLine(airX, airY, airX + airLength, airY + 10, TFT_CYAN);    // 気流のライン1
//     sprite.drawLine(airX, airY + 5, airX + airLength, airY + 15, TFT_CYAN); // 気流のライン2
//     sprite.drawLine(airX, airY + 10, airX + airLength, airY + 20, TFT_CYAN); // 気流のライン3
// }

#endif // ICON_HPP_